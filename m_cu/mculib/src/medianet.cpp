#include "medianet.h"

#include "usageenvironment.hh"
#include "BasicUsageEnvironment.hh"
#include "GroupsockHelper.hh"
#include "liveMedia.hh"

#include "h264streamdecodesink.h"
#include "mpeg4streamdecodesink.h"
#include "mcudefine.h"
#include "log.h"
#include "mcuconfig.h"
#include "mcucommon.h"

#if defined( _WIN32_WCE ) || defined( __SYMBIAN32__ )
#include "SDL.h"
#else
#include <SDL/SDL.h>
#endif

Medium* createClient(UsageEnvironment& env,
					 int verbosityLevel, char const* applicationName) {
						 extern portNumBits tunnelOverHTTPPortNum;
						 return RTSPClient::createNew(env, verbosityLevel, applicationName,
							 tunnelOverHTTPPortNum);
}

char* getOptionsResponse(Medium* client, char const* url,
						 char* username, char* password) {
							 RTSPClient* rtspClient = (RTSPClient*)client;
							 return rtspClient->sendOptionsCmd(url, username, password);
}

char* getSDPDescriptionFromURL(Medium* client, char const* url,
							   char const* username, char const* password,
							   char const* /*proxyServerName*/,
							   unsigned short /*proxyServerPortNum*/,
							   unsigned short /*clientStartPort*/) {
								   RTSPClient* rtspClient = (RTSPClient*)client;
								   char* result;
								   if (username != NULL && password != NULL) {
									   result = rtspClient->describeWithPassword(url, username, password);
								   } else {
									   result = rtspClient->describeURL(url);
								   }

								   extern unsigned statusCode;
								   statusCode = rtspClient->describeStatus();
								   return result;
}

BOOL clientSetupSubsession(Medium* client, MediaSubsession* subsession,
							  BOOL streamUsingTCP, unsigned *pResponseCode /*= NULL*/) {
								  if (client == NULL || subsession == NULL) return False;
								  RTSPClient* rtspClient = (RTSPClient*)client;
								  return rtspClient->setupMediaSubsession(*subsession,
									  False, streamUsingTCP, FALSE, pResponseCode);
}

BOOL clientStartPlayingSession(Medium* client,
								  MediaSession* session) {
									  extern double initialSeekTime, duration, scale;
									  double endTime = initialSeekTime;
									  if (scale > 0) {
										  if (duration <= 0) endTime = -1.0f;
										  else endTime = initialSeekTime + duration;
									  } else {
										  endTime = initialSeekTime - duration;
										  if (endTime < 0) endTime = 0.0f;
									  }

									  if (client == NULL || session == NULL) return False;
									  RTSPClient* rtspClient = (RTSPClient*)client;
									  return rtspClient->playMediaSession(*session, initialSeekTime, endTime, (float)scale);
}

BOOL clientTearDownSession(Medium* client,
							  MediaSession* session) {
								  if (client == NULL || session == NULL) return False;
								  RTSPClient* rtspClient = (RTSPClient*)client;
								  return rtspClient->teardownMediaSession(*session);
}

BOOL allowProxyServers = False;
BOOL controlConnectionUsesTCP = True;
BOOL supportCodecSelection = False;
char const* clientProtocolName = "RTSP";


// Forward function definitions:
BOOL setupStreams( unsigned *pResponseCode = NULL );
void startPlayingStreams();
void tearDownStreams();
void closeMediaSinks();
void subsessionAfterPlaying(void* clientData);
void subsessionByeHandler(void* clientData);
void sessionAfterPlaying(void* clientData = NULL);
void sessionTimerHandler(void* clientData);
//void signalHandlerShutdown(int sig);
void checkForPacketArrival(void* clientData);
void checkInterPacketGaps(void* clientData);
void beginQOSMeasurement();

char const* progName;
UsageEnvironment* env;
Medium* ourClient = NULL;
MediaSession* session = NULL;
TaskToken sessionTimerTask = NULL;
TaskToken arrivalCheckTimerTask = NULL;
TaskToken interPacketGapCheckTimerTask = NULL;
TaskToken qosMeasurementTimerTask = NULL;
//BOOL createReceivers = True;
//BOOL outputQuickTimeFile = False;
//BOOL generateMP4Format = False;
//QuickTimeFileSink* qtOut = NULL;
//BOOL outputAVIFile = False;
//AVIFileSink* aviOut = NULL;
//BOOL audioOnly = False;
//BOOL videoOnly = False;
char const* singleMedium = NULL;
int verbosityLevel = 1; // by default, print verbose output
double duration = 0;
double durationSlop = -1.0; // extra seconds to play at the end
double initialSeekTime = 0.0f;
double scale = 1.0f;
unsigned interPacketGapMaxTime = 0;
unsigned totNumPacketsReceived = ~0; // used if checking inter-packet gaps
BOOL playContinuously = False;
int simpleRTPoffsetArg = -1;
BOOL oneFilePerFrame = False;
BOOL notifyOnPacketArrival = False;
BOOL streamUsingTCP = False;
portNumBits tunnelOverHTTPPortNum = 0;
char* username = NULL;
char* password = NULL;
char* proxyServerName = NULL;
unsigned short proxyServerPortNum = 0;
unsigned char desiredAudioRTPPayloadFormat = 0;
char* mimeSubtype = NULL;
unsigned short movieWidth = 240; // default
BOOL movieWidthOptionSet = False;
unsigned short movieHeight = 180; // default
BOOL movieHeightOptionSet = False;
unsigned movieFPS = 15; // default
BOOL movieFPSOptionSet = False;
const char* fileNamePrefix = "";
unsigned fileSinkBufferSize = 20000;
unsigned socketInputBufferSize = 0;
BOOL packetLossCompensate = False;
BOOL syncStreams = False;
BOOL generateHintTracks = False;
unsigned qosMeasurementIntervalMS = 0; // 0 means: Don't output QOS data
unsigned statusCode = 0;

struct timeval startTime;

BOOL setupStreams( unsigned *pResponseCode /*= NULL*/ ) 
{
	MediaSubsessionIterator iter(*session);
	MediaSubsession *subsession;
	BOOL madeProgress = False;

	BOOL bResult = TRUE;
	while ((subsession = iter.next()) != NULL) 
	{
		if (subsession->clientPortNum() == 0) continue; // port # was not set

		if ( !clientSetupSubsession(ourClient, subsession, streamUsingTCP, pResponseCode ) ) 
		{
			*env << "Failed to setup \"" << subsession->mediumName()
				<< "/" << subsession->codecName()
				<< "\" subsession: " << env->getResultMsg() << "\n";
			
			bResult = FALSE;
		}
		else 
		{
			*env << "Setup \"" << subsession->mediumName()
				<< "/" << subsession->codecName()
				<< "\" subsession (client ports " << subsession->clientPortNum()
				<< "-" << subsession->clientPortNum()+1 << ")\n";
			madeProgress = True;

			bResult = TRUE;
		}
	}
	//if (!madeProgress) 
	//	return bResult;
	return bResult;
}

void startPlayingStreams() {
	if (duration == 0) {
		if (scale > 0) duration = session->playEndTime() - initialSeekTime; // use SDP end time
		else if (scale < 0) duration = initialSeekTime;
	}
	if (duration < 0) duration = 0.0;

	if (!clientStartPlayingSession(ourClient, session)) {
		*env << "Failed to start playing session: " << env->getResultMsg() << "\n";
		return;
	} else {
		*env << "Started playing session\n";
	}

	if (qosMeasurementIntervalMS > 0) {
		// Begin periodic QOS measurements:
		beginQOSMeasurement();
	}

	// Figure out how long to delay (if at all) before shutting down, or
	// repeating the playing
	BOOL timerIsBeingUsed = False;
	double secondsToDelay = duration;
	if (duration > 0) {
		double const maxDelayTime
			= (double)( ((unsigned)0x7FFFFFFF)/1000000.0 );
		if (duration > maxDelayTime) {
			*env << "Warning: specified end time " << duration
				<< " exceeds maximum " << maxDelayTime
				<< "; will not do a delayed shutdown\n";
		} else {
			timerIsBeingUsed = True;
			double absScale = scale > 0 ? scale : -scale; // ASSERT: scale != 0
			secondsToDelay = duration/absScale + durationSlop;

			int uSecsToDelay = (int)(secondsToDelay*1000000.0);
			sessionTimerTask = env->taskScheduler().scheduleDelayedTask(
				uSecsToDelay, (TaskFunc*)sessionTimerHandler, (void*)NULL);
		}
	}

	char const* actionString = "Receiving streamed data";
	if (timerIsBeingUsed) {
		*env << actionString
			<< " (for up to " << secondsToDelay
			<< " seconds)...\n";
	} else {
#ifdef USE_SIGNALS
		pid_t ourPid = getpid();
		*env << actionString
			<< " (signal with \"kill -HUP " << (int)ourPid
			<< "\" or \"kill -USR1 " << (int)ourPid
			<< "\" to terminate)...\n";
#else
		*env << actionString << "...\n";
#endif
	}

	// Watch for incoming packets (if desired):
	checkForPacketArrival(NULL);
	checkInterPacketGaps(NULL);
}

void tearDownStreams() {
	if (session == NULL) return;

	clientTearDownSession(ourClient, session);
}

void closeMediaSinks() {
	//Medium::close(qtOut);
	//Medium::close(aviOut);

	if (session == NULL) return;
	MediaSubsessionIterator iter(*session);
	MediaSubsession* subsession;
	while ((subsession = iter.next()) != NULL) {
		Medium::close(subsession->sink);
		subsession->sink = NULL;
	}
}

void subsessionAfterPlaying(void* clientData) {
	// Begin by closing this media subsession's stream:
	MediaSubsession* subsession = (MediaSubsession*)clientData;
	Medium::close(subsession->sink);
	subsession->sink = NULL;

	// Next, check whether *all* subsessions' streams have now been closed:
	MediaSession& session = subsession->parentSession();
	MediaSubsessionIterator iter(session);
	while ((subsession = iter.next()) != NULL) {
		if (subsession->sink != NULL) return; // this subsession is still active
	}

	// All subsessions' streams have now been closed
	sessionAfterPlaying();
}

void subsessionByeHandler(void* clientData) {
	struct timeval timeNow;
	gettimeofday(&timeNow, NULL);
	unsigned secsDiff = timeNow.tv_sec - startTime.tv_sec;

	MediaSubsession* subsession = (MediaSubsession*)clientData;
	*env << "Received RTCP \"BYE\" on \"" << subsession->mediumName()
		<< "/" << subsession->codecName()
		<< "\" subsession (after " << secsDiff
		<< " seconds)\n";

	// Act now as if the subsession had closed:
	subsessionAfterPlaying(subsession);
}

void sessionAfterPlaying(void* /*clientData*/) {
	if (!playContinuously) {
		return;
	} else {
		// We've been asked to play the stream(s) over again:
		startPlayingStreams();
	}
}

void sessionTimerHandler(void* /*clientData*/) {
	sessionTimerTask = NULL;

	sessionAfterPlaying();
}

class qosMeasurementRecord {
public:
	qosMeasurementRecord(struct timeval const& startTime, RTPSource* src)
		: fSource(src), fNext(NULL),
		kbits_per_second_min(1e20), kbits_per_second_max(0),
		kBytesTotal(0.0),
		packet_loss_fraction_min(1.0), packet_loss_fraction_max(0.0),
		totNumPacketsReceived(0), totNumPacketsExpected(0) {
			measurementEndTime = measurementStartTime = startTime;

#ifdef SUPPORT_REAL_RTSP
			if (session->isRealNetworksRDT) { // hack for RealMedia sessions (RDT, not RTP)
				RealRDTSource* rdt = (RealRDTSource*)src;
				kBytesTotal = rdt->totNumKBytesReceived();
				totNumPacketsReceived = rdt->totNumPacketsReceived();
				totNumPacketsExpected = totNumPacketsReceived; // because we use TCP
				return;
			}
#endif
			RTPReceptionStatsDB::Iterator statsIter(src->receptionStatsDB());
			// Assume that there's only one SSRC source (usually the case):
			RTPReceptionStats* stats = statsIter.next(True);
			if (stats != NULL) {
				kBytesTotal = stats->totNumKBytesReceived();
				totNumPacketsReceived = stats->totNumPacketsReceived();
				totNumPacketsExpected = stats->totNumPacketsExpected();
			}
	}
	virtual ~qosMeasurementRecord() { delete fNext; }

	void periodicQOSMeasurement(struct timeval const& timeNow);

public:
	RTPSource* fSource;
	qosMeasurementRecord* fNext;

public:
	struct timeval measurementStartTime, measurementEndTime;
	double kbits_per_second_min, kbits_per_second_max;
	double kBytesTotal;
	double packet_loss_fraction_min, packet_loss_fraction_max;
	unsigned totNumPacketsReceived, totNumPacketsExpected;
};

static qosMeasurementRecord* qosRecordHead = NULL;

static void periodicQOSMeasurement(void* clientData); // forward

static unsigned nextQOSMeasurementUSecs;

static void scheduleNextQOSMeasurement() {
	nextQOSMeasurementUSecs += qosMeasurementIntervalMS*1000;
	struct timeval timeNow;
	gettimeofday(&timeNow, NULL);
	unsigned timeNowUSecs = timeNow.tv_sec*1000000 + timeNow.tv_usec;
	unsigned usecsToDelay = nextQOSMeasurementUSecs - timeNowUSecs;
	// Note: This works even when nextQOSMeasurementUSecs wraps around

	qosMeasurementTimerTask = env->taskScheduler().scheduleDelayedTask(
		usecsToDelay, (TaskFunc*)periodicQOSMeasurement, (void*)NULL);
}

static void periodicQOSMeasurement(void* /*clientData*/) {
	struct timeval timeNow;
	gettimeofday(&timeNow, NULL);

	for (qosMeasurementRecord* qosRecord = qosRecordHead;
		qosRecord != NULL; qosRecord = qosRecord->fNext) {
			qosRecord->periodicQOSMeasurement(timeNow);
	}

	// Do this again later:
	scheduleNextQOSMeasurement();
}

void qosMeasurementRecord
::periodicQOSMeasurement(struct timeval const& timeNow) {
	unsigned secsDiff = timeNow.tv_sec - measurementEndTime.tv_sec;
	int usecsDiff = timeNow.tv_usec - measurementEndTime.tv_usec;
	double timeDiff = secsDiff + usecsDiff/1000000.0;
	measurementEndTime = timeNow;

#ifdef SUPPORT_REAL_RTSP
	if (session->isRealNetworksRDT) { // hack for RealMedia sessions (RDT, not RTP)
		RealRDTSource* rdt = (RealRDTSource*)fSource;
		double kBytesTotalNow = rdt->totNumKBytesReceived();
		double kBytesDeltaNow = kBytesTotalNow - kBytesTotal;
		kBytesTotal = kBytesTotalNow;

		double kbpsNow = timeDiff == 0.0 ? 0.0 : 8*kBytesDeltaNow/timeDiff;
		if (kbpsNow < 0.0) kbpsNow = 0.0; // in case of roundoff error
		if (kbpsNow < kbits_per_second_min) kbits_per_second_min = kbpsNow;
		if (kbpsNow > kbits_per_second_max) kbits_per_second_max = kbpsNow;

		totNumPacketsReceived = rdt->totNumPacketsReceived();
		totNumPacketsExpected = totNumPacketsReceived; // because we use TCP
		packet_loss_fraction_min = packet_loss_fraction_max = 0.0; // ditto
		return;
	}
#endif
	RTPReceptionStatsDB::Iterator statsIter(fSource->receptionStatsDB());
	// Assume that there's only one SSRC source (usually the case):
	RTPReceptionStats* stats = statsIter.next(True);
	if (stats != NULL) {
		double kBytesTotalNow = stats->totNumKBytesReceived();
		double kBytesDeltaNow = kBytesTotalNow - kBytesTotal;
		kBytesTotal = kBytesTotalNow;

		double kbpsNow = timeDiff == 0.0 ? 0.0 : 8*kBytesDeltaNow/timeDiff;
		if (kbpsNow < 0.0) kbpsNow = 0.0; // in case of roundoff error
		if (kbpsNow < kbits_per_second_min) kbits_per_second_min = kbpsNow;
		if (kbpsNow > kbits_per_second_max) kbits_per_second_max = kbpsNow;

		unsigned totReceivedNow = stats->totNumPacketsReceived();
		unsigned totExpectedNow = stats->totNumPacketsExpected();
		unsigned deltaReceivedNow = totReceivedNow - totNumPacketsReceived;
		unsigned deltaExpectedNow = totExpectedNow - totNumPacketsExpected;
		totNumPacketsReceived = totReceivedNow;
		totNumPacketsExpected = totExpectedNow;

		double lossFractionNow = deltaExpectedNow == 0 ? 0.0
			: 1.0 - deltaReceivedNow/(double)deltaExpectedNow;
		//if (lossFractionNow < 0.0) lossFractionNow = 0.0; //reordering can cause
		if (lossFractionNow < packet_loss_fraction_min) {
			packet_loss_fraction_min = lossFractionNow;
		}
		if (lossFractionNow > packet_loss_fraction_max) {
			packet_loss_fraction_max = lossFractionNow;
		}
	}
}

void beginQOSMeasurement() {
	// Set up a measurement record for each active subsession:
	struct timeval startTime;
	gettimeofday(&startTime, NULL);
	nextQOSMeasurementUSecs = startTime.tv_sec*1000000 + startTime.tv_usec;
	qosMeasurementRecord* qosRecordTail = NULL;
	MediaSubsessionIterator iter(*session);
	MediaSubsession* subsession;
	while ((subsession = iter.next()) != NULL) {
		RTPSource* src = subsession->rtpSource();
#ifdef SUPPORT_REAL_RTSP
		if (session->isRealNetworksRDT) src = (RTPSource*)(subsession->readSource()); // hack
#endif
		if (src == NULL) continue;

		qosMeasurementRecord* qosRecord
			= new qosMeasurementRecord(startTime, src);
		if (qosRecordHead == NULL) qosRecordHead = qosRecord;
		if (qosRecordTail != NULL) qosRecordTail->fNext = qosRecord;
		qosRecordTail  = qosRecord;
	}

	// Then schedule the first of the periodic measurements:
	scheduleNextQOSMeasurement();
}

void printQOSData(int exitCode) {
	if (exitCode != 0 && statusCode == 0) statusCode = 2;
	*env << "begin_QOS_statistics\n";
	*env << "server_availability\t" << (statusCode == 1 ? 0 : 100) << "\n";
	*env << "stream_availability\t" << (statusCode == 0 ? 100 : 0) << "\n";

	// Print out stats for each active subsession:
	qosMeasurementRecord* curQOSRecord = qosRecordHead;
	if (session != NULL) {
		MediaSubsessionIterator iter(*session);
		MediaSubsession* subsession;
		while ((subsession = iter.next()) != NULL) {
			RTPSource* src = subsession->rtpSource();
#ifdef SUPPORT_REAL_RTSP
			if (session->isRealNetworksRDT) src = (RTPSource*)(subsession->readSource()); // hack
#endif
			if (src == NULL) continue;

			*env << "subsession\t" << subsession->mediumName()
				<< "/" << subsession->codecName() << "\n";

			unsigned numPacketsReceived = 0, numPacketsExpected = 0;

			if (curQOSRecord != NULL) {
				numPacketsReceived = curQOSRecord->totNumPacketsReceived;
				numPacketsExpected = curQOSRecord->totNumPacketsExpected;
			}
			*env << "num_packets_received\t" << numPacketsReceived << "\n";
			*env << "num_packets_lost\t" << numPacketsExpected - numPacketsReceived << "\n";

			if (curQOSRecord != NULL) {
				unsigned secsDiff = curQOSRecord->measurementEndTime.tv_sec
					- curQOSRecord->measurementStartTime.tv_sec;
				int usecsDiff = curQOSRecord->measurementEndTime.tv_usec
					- curQOSRecord->measurementStartTime.tv_usec;
				double measurementTime = secsDiff + usecsDiff/1000000.0;
				*env << "elapsed_measurement_time\t" << measurementTime << "\n";

				*env << "kBytes_received_total\t" << curQOSRecord->kBytesTotal << "\n";

				*env << "measurement_sampling_interval_ms\t" << qosMeasurementIntervalMS << "\n";

				if (curQOSRecord->kbits_per_second_max == 0) {
					// special case: we didn't receive any data:
					*env <<
						"kbits_per_second_min\tunavailable\n"
						"kbits_per_second_ave\tunavailable\n"
						"kbits_per_second_max\tunavailable\n";
				} else {
					*env << "kbits_per_second_min\t" << curQOSRecord->kbits_per_second_min << "\n";
					*env << "kbits_per_second_ave\t"
						<< (measurementTime == 0.0 ? 0.0 : 8*curQOSRecord->kBytesTotal/measurementTime) << "\n";
					*env << "kbits_per_second_max\t" << curQOSRecord->kbits_per_second_max << "\n";
				}

				*env << "packet_loss_percentage_min\t" << 100*curQOSRecord->packet_loss_fraction_min << "\n";
				double packetLossFraction = numPacketsExpected == 0 ? 1.0
					: 1.0 - numPacketsReceived/(double)numPacketsExpected;
				if (packetLossFraction < 0.0) packetLossFraction = 0.0;
				*env << "packet_loss_percentage_ave\t" << 100*packetLossFraction << "\n";
				*env << "packet_loss_percentage_max\t"
					<< (packetLossFraction == 1.0 ? 100.0 : 100*curQOSRecord->packet_loss_fraction_max) << "\n";

#ifdef SUPPORT_REAL_RTSP
				if (session->isRealNetworksRDT) {
					RealRDTSource* rdt = (RealRDTSource*)src;
					*env << "inter_packet_gap_ms_min\t" << rdt->minInterPacketGapUS()/1000.0 << "\n";
					struct timeval totalGaps = rdt->totalInterPacketGaps();
					double totalGapsMS = totalGaps.tv_sec*1000.0 + totalGaps.tv_usec/1000.0;
					unsigned totNumPacketsReceived = rdt->totNumPacketsReceived();
					*env << "inter_packet_gap_ms_ave\t"
						<< (totNumPacketsReceived == 0 ? 0.0 : totalGapsMS/totNumPacketsReceived) << "\n";
					*env << "inter_packet_gap_ms_max\t" << rdt->maxInterPacketGapUS()/1000.0 << "\n";
				} else {
#endif
					RTPReceptionStatsDB::Iterator statsIter(src->receptionStatsDB());
					// Assume that there's only one SSRC source (usually the case):
					RTPReceptionStats* stats = statsIter.next(True);
					if (stats != NULL) {
						*env << "inter_packet_gap_ms_min\t" << stats->minInterPacketGapUS()/1000.0 << "\n";
						struct timeval totalGaps = stats->totalInterPacketGaps();
						double totalGapsMS = totalGaps.tv_sec*1000.0 + totalGaps.tv_usec/1000.0;
						unsigned totNumPacketsReceived = stats->totNumPacketsReceived();
						*env << "inter_packet_gap_ms_ave\t"
							<< (totNumPacketsReceived == 0 ? 0.0 : totalGapsMS/totNumPacketsReceived) << "\n";
						*env << "inter_packet_gap_ms_max\t" << stats->maxInterPacketGapUS()/1000.0 << "\n";
					}
#ifdef SUPPORT_REAL_RTSP
				}
#endif

				curQOSRecord = curQOSRecord->fNext;
			}
		}
	}

	*env << "end_QOS_statistics\n";
	delete qosRecordHead;
}

//void shutdown(int exitCode) {
//	if (env != NULL) {
//		env->taskScheduler().unscheduleDelayedTask(sessionTimerTask);
//		env->taskScheduler().unscheduleDelayedTask(arrivalCheckTimerTask);
//		env->taskScheduler().unscheduleDelayedTask(interPacketGapCheckTimerTask);
//		env->taskScheduler().unscheduleDelayedTask(qosMeasurementTimerTask);
//	}
//
//	if (qosMeasurementIntervalMS > 0) {
//		printQOSData(exitCode);
//	}
//
//	// Close our output files:
//	closeMediaSinks();
//
//	// Teardown, then shutdown, any outstanding RTP/RTCP subsessions
//	tearDownStreams();
//	Medium::close(session);
//
//	// Finally, shut down our client:
//	Medium::close(ourClient);
//
//	// Adios...
//	exit(exitCode);
//}

//void signalHandlerShutdown(int /*sig*/) {
//	*env << "Got shutdown signal\n";
//	shutdown(0);
//}

void checkForPacketArrival(void* /*clientData*/) {
	if (!notifyOnPacketArrival) return; // we're not checking

	// Check each subsession, to see whether it has received data packets:
	unsigned numSubsessionsChecked = 0;
	unsigned numSubsessionsWithReceivedData = 0;
	unsigned numSubsessionsThatHaveBeenSynced = 0;

	MediaSubsessionIterator iter(*session);
	MediaSubsession* subsession;
	while ((subsession = iter.next()) != NULL) {
		RTPSource* src = subsession->rtpSource();
		if (src == NULL) continue;
		++numSubsessionsChecked;

		if (src->receptionStatsDB().numActiveSourcesSinceLastReset() > 0) {
			// At least one data packet has arrived
			++numSubsessionsWithReceivedData;
		}
		if (src->hasBeenSynchronizedUsingRTCP()) {
			++numSubsessionsThatHaveBeenSynced;
		}
	}

	unsigned numSubsessionsToCheck = numSubsessionsChecked;
	// Special case for "QuickTimeFileSink"s and "AVIFileSink"s:
	// They might not use all of the input sources:
	//if (qtOut != NULL) {
	//	numSubsessionsToCheck = qtOut->numActiveSubsessions();
	//} else if (aviOut != NULL) {
	//	numSubsessionsToCheck = aviOut->numActiveSubsessions();
	//}

	BOOL notifyTheUser;
	if (!syncStreams) {
		notifyTheUser = numSubsessionsWithReceivedData > 0; // easy case
	} else {
		notifyTheUser = numSubsessionsWithReceivedData >= numSubsessionsToCheck
			&& numSubsessionsThatHaveBeenSynced == numSubsessionsChecked;
		// Note: A subsession with no active sources is considered to be synced
	}
	if (notifyTheUser) {
		struct timeval timeNow;
		gettimeofday(&timeNow, NULL);
		char timestampStr[100];
		sprintf(timestampStr, "%ld%03ld", timeNow.tv_sec, (long)(timeNow.tv_usec/1000));
		*env << (syncStreams ? "Synchronized d" : "D")
			<< "ata packets have begun arriving [" << timestampStr << "]\007\n";
		return;
	}

	// No luck, so reschedule this check again, after a delay:
	int uSecsToDelay = 100000; // 100 ms
	arrivalCheckTimerTask
		= env->taskScheduler().scheduleDelayedTask(uSecsToDelay,
		(TaskFunc*)checkForPacketArrival, NULL);
}

void checkInterPacketGaps(void* /*clientData*/) {
	if (interPacketGapMaxTime == 0) return; // we're not checking

	// Check each subsession, counting up how many packets have been received:
	unsigned newTotNumPacketsReceived = 0;

	MediaSubsessionIterator iter(*session);
	MediaSubsession* subsession;
	while ((subsession = iter.next()) != NULL) {
		RTPSource* src = subsession->rtpSource();
		if (src == NULL) continue;
		newTotNumPacketsReceived += src->receptionStatsDB().totNumPacketsReceived();
	}

	if (newTotNumPacketsReceived == totNumPacketsReceived) {
		// No additional packets have been received since the last time we
		// checked, so end this stream:
		*env << "Closing session, because we stopped receiving packets.\n";
		interPacketGapCheckTimerTask = NULL;
		sessionAfterPlaying();
	} else {
		totNumPacketsReceived = newTotNumPacketsReceived;
		// Check again, after the specified delay:
		interPacketGapCheckTimerTask
			= env->taskScheduler().scheduleDelayedTask(interPacketGapMaxTime*1000000,
			(TaskFunc*)checkInterPacketGaps, NULL);
	}
}


CMediaNet::CMediaNet(void)
{
	m_runFlag = 0;
	m_pMediaNetThread = 0;
	m_eRtspStatus = RTSPStatus_Idle;
}

CMediaNet::~CMediaNet(void)
{
}

BOOL CMediaNet::OpenRTSP( LPCTSTR strRtspUrl )
{
	if ( m_pMediaNetThread )
	{
		this->CloseRTSP();
	}

	
#ifdef _UNICODE	
    m_strRTSPUrlA = ::UTF16toUTF8( strRtspUrl );
#else
	strcpy( multiVideoSrc, strRtspUrl );
    this->m_strRTSPUrlA = strRtspUrl;
#endif

	m_runFlag = 0;
	this->SetRtspStatus( RTSPStatus_Init );

	m_pMediaNetThread = SDL_CreateThread( MediaNet_Thread, this );

	return TRUE;
}

BOOL CMediaNet::CloseRTSP()
{
	// �˳��̡߳�	
	if ( m_pMediaNetThread && GetRtspStatus() != RTSPStatus_Idle )
	{
		m_runFlag = 1;
		int nStatus;

		Log() << _T( "before wait m_pMediaNetThread" ) << m_pMediaNetThread << endl;
		SDL_WaitThread( m_pMediaNetThread, &nStatus );

		Log() << _T( "after wait m_pMediaNetThread" ) << m_pMediaNetThread << endl;

		m_pMediaNetThread = 0;
		SetRtspStatus( RTSPStatus_Idle );
	}
	

	if (env != NULL) {
		env->taskScheduler().unscheduleDelayedTask(sessionTimerTask);
		env->taskScheduler().unscheduleDelayedTask(arrivalCheckTimerTask);
		env->taskScheduler().unscheduleDelayedTask(interPacketGapCheckTimerTask);
		env->taskScheduler().unscheduleDelayedTask(qosMeasurementTimerTask);
	}

	int exitCode = 0;
	if (qosMeasurementIntervalMS > 0) {
		printQOSData(exitCode);
	}

	// Close our output files:
	closeMediaSinks();

	// Teardown, then shutdown, any outstanding RTP/RTCP subsessions
	tearDownStreams();
	Medium::close(session);



	// Finally, shut down our client:
	Medium::close(ourClient);

	session = NULL;
	ourClient = NULL;

	if ( env )
	{
		delete &( env->taskScheduler() );
		env->reclaim();
	}	

	env = NULL;

	return TRUE;
}

int CMediaNet::MediaNet_Thread( void * pThisVoid )
{
	CMediaNet *pThis = ( CMediaNet* )pThisVoid;

	do 
	{
		// ��ʼ��ʼ��.
		pThis->SetRtspStatus( RTSPStatus_Init );

		// Begin by setting up our usage environment:
		TaskScheduler* scheduler = BasicTaskScheduler::createNew();
		env = BasicUsageEnvironment::createNew(*scheduler);

		progName = "M_CU";

		string strUrl = pThis->m_strRTSPUrlA;

		gettimeofday(&startTime, NULL);

		unsigned short desiredPortNum = 0;

		// unfortunately we can't use getopt() here, as Windoze doesn't have it

		// Create our client object:
		ourClient = createClient(*env, verbosityLevel, progName);
		if (ourClient == NULL) 
		{
			*env << "Failed to create " << clientProtocolName
				<< " client: " << env->getResultMsg() << "\n";

			pThis->SetRtspStatus( RTSPStatus_Error_Init_Fail );
			break;
		}
		
#if defined( __SYMBIAN32__ )
		char* sdpDescription3
					= getSDPDescriptionFromURL(ourClient, pThis->m_strRTSPUrlA.c_str(), username, password,
					proxyServerName, proxyServerPortNum,
					desiredPortNum);
				if (sdpDescription3 == NULL) 
				{
					*env << "Failed to get a SDP description from URL \"" << pThis->m_strRTSPUrlA.c_str()
						<< "\": " << env->getResultMsg() << "\n";

					pThis->SetRtspStatus( RTSPStatus_Error_Description  );
					break;
				}
#endif

		// ��ʼ��ȡOpition.
		pThis->SetRtspStatus( RTSPStatus_Opition );
		// Begin by sending an "OPTIONS" command:
		char* optionsResponse
			= getOptionsResponse(ourClient, pThis->m_strRTSPUrlA.c_str(), username, password);

		if (optionsResponse == NULL) 
		{
			*env << clientProtocolName << " \"OPTIONS\" request failed: "
				<< env->getResultMsg() << "\n";

			pThis->SetRtspStatus( RTSPStatus_Error_Opition );
			break;
		} 
		else 
		{
			*env << clientProtocolName << " \"OPTIONS\" request returned: "
				<< optionsResponse << "\n";
		}
		if( optionsResponse )
		{
			delete[] optionsResponse;
		}
			

		// ��ʼ��ȡDescription.
		// Open the URL, to get a SDP description:
		pThis->SetRtspStatus( RTSPStatus_Description );
		char* sdpDescription
			= getSDPDescriptionFromURL(ourClient, pThis->m_strRTSPUrlA.c_str(), username, password,
			proxyServerName, proxyServerPortNum,
			desiredPortNum);
		if (sdpDescription == NULL) 
		{
			*env << "Failed to get a SDP description from URL \"" << pThis->m_strRTSPUrlA.c_str()
				<< "\": " << env->getResultMsg() << "\n";

			pThis->SetRtspStatus( RTSPStatus_Error_Description  );
			break;
		}

		*env << "Opened URL \"" << pThis->m_strRTSPUrlA.c_str()
			<< "\", returning a SDP description:\n" << sdpDescription << "\n";

		// Create a media session object from this SDP description:
		session = MediaSession::createNew(*env, sdpDescription);
		delete[] sdpDescription;
		if (session == NULL) 
		{
			*env << "Failed to create a MediaSession object from the SDP description: " << env->getResultMsg() << "\n";

			pThis->SetRtspStatus( RTSPStatus_Error_SDP );
			break;
		} 
		else if (!session->hasSubsessions()) 
		{
			*env << "This session has no media subsessions (i.e., \"m=\" lines)\n";

			pThis->SetRtspStatus( RTSPStatus_Error_SDP );
			break;
		}

		// Then, setup the "RTPSource"s for the session:
		MediaSubsessionIterator iter(*session);
		MediaSubsession *subsession;
		BOOL madeProgress = False;
		char const* singleMediumToTest = singleMedium;
		while ((subsession = iter.next()) != NULL) 
		{
			// If we've asked to receive only a single medium, then check this now:
			if (singleMediumToTest != NULL) 
			{
				if (strcmp(subsession->mediumName(), singleMediumToTest) != 0) 
				{
					*env << "Ignoring \"" << subsession->mediumName()
						<< "/" << subsession->codecName()
						<< "\" subsession, because we've asked to receive a single " << singleMedium
						<< " session only\n";
					continue;
				} 
				else 
				{
					// Receive this subsession only
					singleMediumToTest = "xxxxx";
					// this hack ensures that we get only 1 subsession of this type
				}
			}

			desiredPortNum = 0;
			if (desiredPortNum != 0) 
			{
				subsession->setClientPortNum(desiredPortNum);
				desiredPortNum += 2;
			}

			if (true) 
			{
				if (!subsession->initiate(simpleRTPoffsetArg)) 
				{
					*env << "Unable to create receiver for \"" << subsession->mediumName()
						<< "/" << subsession->codecName()
						<< "\" subsession: " << env->getResultMsg() << "\n";
				} 
				else 
				{
					*env << "Created receiver for \"" << subsession->mediumName()
						<< "/" << subsession->codecName()
						<< "\" subsession (client ports " << subsession->clientPortNum()
						<< "-" << subsession->clientPortNum()+1 << ")\n";
					madeProgress = True;

					if (subsession->rtpSource() != NULL) 
					{
						// Because we're saving the incoming data, rather than playing
						// it in real time, allow an especially large time threshold
						// (1 second) for reordering misordered incoming packets:
						unsigned const thresh = 1000000; // 1 second
						subsession->rtpSource()->setPacketReorderingThresholdTime(thresh);

						if (socketInputBufferSize > 0) 
						{
							// Set the RTP source's input buffer size as specified:
							int socketNum
								= subsession->rtpSource()->RTPgs()->socketNum();
							unsigned curBufferSize
								= getReceiveBufferSize(*env, socketNum);
							unsigned newBufferSize
								= setReceiveBufferTo(*env, socketNum, socketInputBufferSize);
							*env << "Changed socket receive buffer size for the \""
								<< subsession->mediumName()
								<< "/" << subsession->codecName()
								<< "\" subsession from "
								<< curBufferSize << " to "
								<< newBufferSize << " bytes\n";
						}
					}
				}
			} 
			else 
			{
				Log() << _T( "Use port: " ) << (int)subsession->clientPortNum() << endl;
				if (subsession->clientPortNum() == 0) 
				{
					*env << "No client port was specified for the \""
						<< subsession->mediumName()
						<< "/" << subsession->codecName()
						<< "\" subsession.  (Try adding the \"-p <portNum>\" option.)\n";
				} 
				else 
				{
					madeProgress = True;
				}
			}
		}
		if (!madeProgress) 
		{
			pThis->SetRtspStatus( RTSPStatus_Error_Create_Rcv );
			break;
		}

		// Perform additional 'setup' on each subsession, before playing them:
		pThis->SetRtspStatus( RTSPStatus_Setup );
		unsigned nResponseCode = NULL;
		BOOL bSetupSuccess = setupStreams( &nResponseCode );
		if ( !bSetupSuccess )
		{
			// setupʧ�ܣ�
			if ( RTSPResp_Error_Server_Full == nResponseCode )
			{
				pThis->SetRtspStatus( RTSPStatus_Error_Server_Full );
			}
			else
			{
				pThis->SetRtspStatus( RTSPStatus_Error_Setup );
			}
			break;
		}
		// Create output files:
		
		// Create and start "FileSink"s for each subsession: 
		madeProgress = False;
		iter.reset();
		while ((subsession = iter.next()) != NULL) 
		{
			if (subsession->readSource() == NULL) continue; // was not initiated

			MediaSink *pDecodeSink = 0;
			if (strcmp(subsession->mediumName(), "video") == 0 )
			{
				int nBandWidth = subsession->GetBandWidth();

				if ( strcmp(subsession->codecName(), "MP4V-ES") == 0 )
				{
					CMpeg4StreamDecodeSink *pMsds = CMpeg4StreamDecodeSink::CreateNew( *env, 20000, nBandWidth );
					 pDecodeSink = pMsds;
					
				}
				else if ( strcmp( subsession->codecName(), "H264" ) == 0 )
				{
					 CH264StreamDecodeSink *pHsds = CH264StreamDecodeSink::CreateNew( *env, 20000, nBandWidth );
					 pDecodeSink = pHsds;
				}
				else
				{
					continue;
				}

				

			}

			

			

			subsession->sink = pDecodeSink;
			if (subsession->sink == NULL) 
			{
				*env << "Failed to create CH264StreamDecodeSink \""  << "\n";
			} 


			subsession->sink->startPlaying(*(subsession->readSource()),
				subsessionAfterPlaying,
				subsession);

			// Also set a handler to be called if a RTCP "BYE" arrives
			// for this subsession:
			if (subsession->rtcpInstance() != NULL) 
			{
				subsession->rtcpInstance()->setByeHandler(subsessionByeHandler,
					subsession);
			}

			// ����NAT̽�������ֹ��������3�Ρ�
            BOOL bSendNatPacket;
            CConfig::Instance()->GetIsSendNatPacket( bSendNatPacket );
            if( bSendNatPacket )
            {
			    unsigned char temp[112] = {0};
			    temp[0] = 0x80;
			    for ( int i=0; i<3; ++i )
			    {
				    subsession->rtpSource()->RTPgs()->output( *env, 0,temp, 112 );
			    }
                Log() << _T( "Send Nat packet!" ) << endl;
            }
            else
            {
                Log() << _T( "Not send nat packet!" ) << endl;
            }
			

			madeProgress = True;
		}

        // ����Ƿ񴴽��˽�������
        if( NULL == CDecoder::FindDecoder( MAIN_DECODER_NAME ) )
        {
            pThis->SetRtspStatus( RTSPStatus_Error_Decoder_Fail );
            break;
        }


		// Finally, start playing each subsession, to start the data flow:
		pThis->SetRtspStatus( RTSPStatus_Play );
		startPlayingStreams();


		pThis->SetRtspStatus( RTSPStatus_Running );
		// ���������־ָ�롣 
		env->taskScheduler().doEventLoop( &pThis->m_runFlag ); 

		pThis->SetRtspStatus( RTSPStatus_Idle );

	} while(0);

//	Log() << _T( "MediaNet Thread exit!" ) << endl;

	return 0;
}

void CMediaNet::SetRtspStatus( ERTSPStatus eStat )
{
	m_eRtspStatus = eStat;
}

ERTSPStatus CMediaNet::GetRtspStatus() const
{
	return m_eRtspStatus;
}










