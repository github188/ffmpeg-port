

/*
* Copyright (c) 2000, 2001, 2002 Fabrice Bellard
*
* This file is part of FFmpeg.
*
* FFmpeg is free software; you can redistribute it and/or
* modify it under the terms of the GNU Lesser General Public
* License as published by the Free Software Foundation; either
* version 2.1 of the License, or (at your option) any later version.
*
* FFmpeg is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
* Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public
* License along with FFmpeg; if not, write to the Free Software
* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
*/
#include "avformat.h"
#include "libavcodec/opt.h"

#include "riff.h"
#include "rational.h"
#include "define.h"
/**
* @file libavformat/options.c
* Options definition for AVFormatContext.
*/

static const char* format_to_name(void* ptr)
{
	AVFormatContext* fc = (AVFormatContext*) ptr;
	if(fc->iformat) return fc->iformat->name;
	else if(fc->oformat) return fc->oformat->name;
	else return "NULL";
}

#define OFFSET(x) offsetof(AVFormatContext,x)
#define DEFAULT 0 //should be NAN but it does not work as it is not a constant in glibc as required by ANSI/ISO C
//these names are too long to be readable
#define E AV_OPT_FLAG_ENCODING_PARAM
#define D AV_OPT_FLAG_DECODING_PARAM

static const AVOption options[]={
	{"probesize", NULL, OFFSET(probesize), FF_OPT_TYPE_INT, 32000, 32, INT_MAX, D}, /* 32000 from mpegts.c: 1.0 second at 24Mbit/s */
	{"muxrate", "set mux rate", OFFSET(mux_rate), FF_OPT_TYPE_INT, DEFAULT, 0, INT_MAX, E},
	{"packetsize", "set packet size", OFFSET(packet_size), FF_OPT_TYPE_INT, DEFAULT, 0, INT_MAX, E},
	{"fflags", NULL, OFFSET(flags), FF_OPT_TYPE_FLAGS, DEFAULT, INT_MIN, INT_MAX, D|E, "fflags"},
	{"ignidx", "ignore index", 0, FF_OPT_TYPE_CONST, AVFMT_FLAG_IGNIDX, INT_MIN, INT_MAX, D, "fflags"},
	{"genpts", "generate pts", 0, FF_OPT_TYPE_CONST, AVFMT_FLAG_GENPTS, INT_MIN, INT_MAX, D, "fflags"},

#ifdef THINKINGL_OLD_VER
	{"track", " set the track number", OFFSET(track), FF_OPT_TYPE_INT, DEFAULT, 0, INT_MAX, E},
	{"year", "set the year", OFFSET(year), FF_OPT_TYPE_INT, DEFAULT, INT_MIN, INT_MAX, E},
#endif

	{"analyzeduration", "how many microseconds are analyzed to estimate duration", OFFSET(max_analyze_duration), FF_OPT_TYPE_INT, 3*AV_TIME_BASE, 0, INT_MAX, D},
	{"cryptokey", "decryption key", OFFSET(key), FF_OPT_TYPE_BINARY, 0, 0, 0, D},
	{"indexmem", "max memory used for timestamp index (per stream)", OFFSET(max_index_size), FF_OPT_TYPE_INT, 1<<20, 0, INT_MAX, D},
	{"rtbufsize", "max memory used for buffering real-time frames", OFFSET(max_picture_buffer), FF_OPT_TYPE_INT, 3041280, 0, INT_MAX, D}, /* defaults to 1s of 15fps 352x288 YUYV422 video */
	{"fdebug", "print specific debug info", OFFSET(debug), FF_OPT_TYPE_FLAGS, DEFAULT, 0, INT_MAX, E|D, "fdebug"},
	{"ts", NULL, 0, FF_OPT_TYPE_CONST, FF_FDEBUG_TS, INT_MIN, INT_MAX, E|D, "fdebug"},
	{NULL},
};

#undef E
#undef D
#undef DEFAULT

static const AVClass av_format_context_class = { "AVFormatContext", format_to_name, options };

static void avformat_get_context_defaults(AVFormatContext *s)
{
	memset(s, 0, sizeof(AVFormatContext));

	s->av_class = &av_format_context_class;

	av_opt_set_defaults(s);
}

AVFormatContext *avformat_alloc_context(void)
{
	AVFormatContext *ic;
	ic = av_malloc(sizeof(AVFormatContext));
	if (!ic) return ic;
	avformat_get_context_defaults(ic);
	ic->av_class = &av_format_context_class;
	return ic;
}

#if LIBAVFORMAT_VERSION_MAJOR < 53
AVFormatContext *av_alloc_format_context(void)
{
	return avformat_alloc_context();
}
#endif

// thinkingl 3gp
int av_get_bits_per_sample(enum CodecID codec_id){
	switch(codec_id){
	case CODEC_ID_ADPCM_CT:
		return 4;
	case CODEC_ID_PCM_ALAW:
	case CODEC_ID_PCM_MULAW:
	case CODEC_ID_PCM_S8:
	case CODEC_ID_PCM_U8:
		return 8;
	case CODEC_ID_PCM_S16BE:
	case CODEC_ID_PCM_S16LE:
	case CODEC_ID_PCM_U16BE:
	case CODEC_ID_PCM_U16LE:
		return 16;
	case CODEC_ID_PCM_S24DAUD:
	case CODEC_ID_PCM_S24BE:
	case CODEC_ID_PCM_S24LE:
	case CODEC_ID_PCM_U24BE:
	case CODEC_ID_PCM_U24LE:
		return 24;
	case CODEC_ID_PCM_S32BE:
	case CODEC_ID_PCM_S32LE:
	case CODEC_ID_PCM_U32BE:
	case CODEC_ID_PCM_U32LE:
		return 32;

	default:
		return 0;
	}
}

void av_set_pts_info(AVStream *s, int pts_wrap_bits,
					 int pts_num, int pts_den)
{
	unsigned int gcd= av_gcd(pts_num, pts_den);
	s->pts_wrap_bits = pts_wrap_bits;
	s->time_base.num = pts_num/gcd;
	s->time_base.den = pts_den/gcd;

	if(gcd>1)
		av_log(NULL, AV_LOG_DEBUG, "st:%d removing common factor %d from timebase\n", s->index, gcd);
}

unsigned int codec_get_tag(const AVCodecTag *tags, int id)
{
	while (tags->id != CODEC_ID_NONE) {
		if (tags->id == id)
			return tags->tag;
		tags++;
	}
	return 0;
}

AVStream *av_new_stream(AVFormatContext *s, int id)
{
	AVStream *st;
	int i;

	if (s->nb_streams >= MAX_STREAMS)
		return NULL;

	st = av_mallocz(sizeof(AVStream));
	if (!st)
		return NULL;

	st->codec= avcodec_alloc_context();
	if ( !st->codec )
	{
		av_log( s, AV_LOG_ERROR, "avcodec_alloc_context fail!" );
		av_free( st );
		return NULL;
	}

	if (s->iformat) {
		/* no default bitrate if decoding */
		st->codec->bit_rate = 0;
	}
	st->index = s->nb_streams;
	st->id = id;
	st->start_time = AV_NOPTS_VALUE;
	st->duration = AV_NOPTS_VALUE;
	/* we set the current DTS to 0 so that formats without any timestamps
	but durations get some timestamps, formats with some unknown
	timestamps have their first few packets buffered and the
	timestamps corrected before they are returned to the user */
	st->cur_dts = 0;
	st->first_dts = AV_NOPTS_VALUE;

	/* default pts setting is MPEG-like */
	av_set_pts_info(st, 33, 1, 90000);
	st->last_IP_pts = AV_NOPTS_VALUE;
	for(i=0; i<MAX_REORDER_DELAY+1; i++)
		st->pts_buffer[i]= AV_NOPTS_VALUE;
	st->reference_dts = AV_NOPTS_VALUE;

	// thinkingl!!
	st->sample_aspect_ratio = _AVRational(0,1);	// (AVRational){0,1};	

	s->streams[s->nb_streams++] = st;
	return st;
}

//AVProgram *av_new_program(AVFormatContext *ac, int id)
//{
//	AVProgram *program=NULL;
//	int i;
//
//#ifdef DEBUG_SI
//	av_log(ac, AV_LOG_DEBUG, "new_program: id=0x%04x\n", id);
//#endif
//
//	for(i=0; i<ac->nb_programs; i++)
//		if(ac->programs[i]->id == id)
//			program = ac->programs[i];
//
//	if(!program){
//		program = av_mallocz(sizeof(AVProgram));
//		if (!program)
//			return NULL;
//		dynarray_add(&ac->programs, &ac->nb_programs, program);
//		program->discard = AVDISCARD_NONE;
//	}
//	program->id = id;
//
//	return program;
//}

/************************************************************/
/* output media file */

int av_set_parameters(AVFormatContext *s, AVFormatParameters *ap)
{
	int ret;

	if (s->oformat->priv_data_size > 0) {
		s->priv_data = av_mallocz(s->oformat->priv_data_size);
		if (!s->priv_data)
			return AVERROR(ENOMEM);
	} else
		s->priv_data = NULL;

	if (s->oformat->set_parameters) {
		ret = s->oformat->set_parameters(s, ap);
		if (ret < 0)
			return ret;
	}
	return 0;
}

/* fraction handling */

/**
* f = val + (num / den) + 0.5.
*
* 'num' is normalized so that it is such as 0 <= num < den.
*
* @param f fractional number
* @param val integer value
* @param num must be >= 0
* @param den must be >= 1
*/
static void av_frac_init(AVFrac *f, int64_t val, int64_t num, int64_t den)
{
	num += (den >> 1);
	if (num >= den) {
		val += num / den;
		num = num % den;
	}
	f->val = val;
	f->num = num;
	f->den = den;
}

/**
* Fractional addition to f: f = f + (incr / f->den).
*
* @param f fractional number
* @param incr increment, can be positive or negative
*/
static void av_frac_add(AVFrac *f, int64_t incr)
{
	int64_t num, den;

	num = f->num + incr;
	den = f->den;
	if (num < 0) {
		f->val += num / den;
		num = num % den;
		if (num < 0) {
			num += den;
			f->val--;
		}
	} else if (num >= den) {
		f->val += num / den;
		num = num % den;
	}
	f->num = num;
}


unsigned int av_codec_get_tag(const AVCodecTag * const *tags, enum CodecID id)
{
	int i;
	for(i=0; tags && tags[i]; i++){
		int tag= codec_get_tag(tags[i], id);
		if(tag) return tag;
	}
	return 0;
}


int av_write_header(AVFormatContext *s)
{
	int ret, i;
	AVStream *st;

	// some sanity checks
	for(i=0;i<s->nb_streams;i++) {
		st = s->streams[i];

		switch (st->codec->codec_type) {
		case CODEC_TYPE_AUDIO:
			if(st->codec->sample_rate<=0){
				av_log(s, AV_LOG_ERROR, "sample rate not set\n");
				return -1;
			}
			if(!st->codec->block_align)
				st->codec->block_align = st->codec->channels *
				av_get_bits_per_sample(st->codec->codec_id) >> 3;
			break;
		case CODEC_TYPE_VIDEO:
			if(st->codec->time_base.num<=0 || st->codec->time_base.den<=0){ //FIXME audio too?
				av_log(s, AV_LOG_ERROR, "time base not set\n");
				return -1;
			}
			if(st->codec->width<=0 || st->codec->height<=0){
				av_log(s, AV_LOG_ERROR, "dimensions not set\n");
				return -1;
			}
			if(av_cmp_q(st->sample_aspect_ratio, st->codec->sample_aspect_ratio)){
				av_log(s, AV_LOG_ERROR, "Aspect ratio mismatch between encoder and muxer layer\n");
				return -1;
			}
			break;
		}

		if(s->oformat->codec_tag){
			if(st->codec->codec_tag){
				//FIXME
				//check that tag + id is in the table
				//if neither is in the table -> OK
				//if tag is in the table with another id -> FAIL
				//if id is in the table with another tag -> FAIL unless strict < ?
			}else
				st->codec->codec_tag= av_codec_get_tag(s->oformat->codec_tag, st->codec->codec_id);
		}

		if(s->oformat->flags & AVFMT_GLOBALHEADER &&
			!(st->codec->flags & CODEC_FLAG_GLOBAL_HEADER))
			av_log(s, AV_LOG_WARNING, "Codec for stream %d does not use global headers but container format requires global headers\n", i);
	}

	if (!s->priv_data && s->oformat->priv_data_size > 0) {
		s->priv_data = av_mallocz(s->oformat->priv_data_size);
		if (!s->priv_data)
			return AVERROR(ENOMEM);
	}

#if LIBAVFORMAT_VERSION_MAJOR < 53
	ff_metadata_mux_compat(s);
#endif

	if(s->oformat->write_header){
		ret = s->oformat->write_header(s);
		if (ret < 0)
			return ret;
	}

	/* init PTS generation */
	for(i=0;i<s->nb_streams;i++) {
		int64_t den = AV_NOPTS_VALUE;
		st = s->streams[i];

		switch (st->codec->codec_type) {
		case CODEC_TYPE_AUDIO:
			den = (int64_t)st->time_base.num * st->codec->sample_rate;
			break;
		case CODEC_TYPE_VIDEO:
			den = (int64_t)st->time_base.num * st->codec->time_base.den;
			break;
		default:
			break;
		}
		if (den != AV_NOPTS_VALUE) {
			if (den <= 0)
				return AVERROR_INVALIDDATA;
			av_frac_init(&st->pts, 0, 0, den);
		}
	}
	return 0;
}

/* memory handling */

void av_destruct_packet(AVPacket *pkt)
{
	av_free(pkt->data);
	pkt->data = NULL; pkt->size = 0;
}

void av_init_packet(AVPacket *pkt)
{
	pkt->pts   = AV_NOPTS_VALUE;
	pkt->dts   = AV_NOPTS_VALUE;
	pkt->pos   = -1;
	pkt->duration = 0;
	pkt->convergence_duration = 0;
	pkt->flags = 0;
	pkt->stream_index = 0;
	pkt->destruct= av_destruct_packet_nofree;
}

int av_new_packet(AVPacket *pkt, int size)
{
	uint8_t *data;
	if((unsigned)size > (unsigned)size + FF_INPUT_BUFFER_PADDING_SIZE)
		return AVERROR(ENOMEM);
	data = av_malloc(size + FF_INPUT_BUFFER_PADDING_SIZE);
	if (!data)
		return AVERROR(ENOMEM);
	memset(data + size, 0, FF_INPUT_BUFFER_PADDING_SIZE);

	av_init_packet(pkt);
	pkt->data = data;
	pkt->size = size;
	pkt->destruct = av_destruct_packet;
	return 0;
}

int av_get_packet(ByteIOContext *s, AVPacket *pkt, int size)
{
	int ret= av_new_packet(pkt, size);

	if(ret<0)
		return ret;

	pkt->pos= url_ftell(s);

	ret= get_buffer(s, pkt->data, size);
	if(ret<=0)
		av_free_packet(pkt);
	else
		pkt->size= ret;

	return ret;
}

int av_dup_packet(AVPacket *pkt)
{
	if (((pkt->destruct == av_destruct_packet_nofree) || (pkt->destruct == NULL)) && pkt->data) {
		uint8_t *data;
		/* We duplicate the packet and don't forget to add the padding again. */
		if((unsigned)pkt->size > (unsigned)pkt->size + FF_INPUT_BUFFER_PADDING_SIZE)
			return AVERROR(ENOMEM);
		data = av_malloc(pkt->size + FF_INPUT_BUFFER_PADDING_SIZE);
		if (!data) {
			return AVERROR(ENOMEM);
		}
		memcpy(data, pkt->data, pkt->size);
		memset(data + pkt->size, 0, FF_INPUT_BUFFER_PADDING_SIZE);
		pkt->data = data;
		pkt->destruct = av_destruct_packet;
	}
	return 0;
}

void av_destruct_packet_nofree(AVPacket *pkt)
{
	pkt->data = NULL; pkt->size = 0;
}


/**********************************************************/

/**
* Get the number of samples of an audio frame. Return -1 on error.
*/
static int get_audio_frame_size(AVCodecContext *enc, int size)
{
	int frame_size;

	if(enc->codec_id == CODEC_ID_VORBIS)
		return -1;

	if (enc->frame_size <= 1) {
		int bits_per_sample = av_get_bits_per_sample(enc->codec_id);

		if (bits_per_sample) {
			if (enc->channels == 0)
				return -1;
			frame_size = (size << 3) / (bits_per_sample * enc->channels);
		} else {
			/* used for example by ADPCM codecs */
			if (enc->bit_rate == 0)
				return -1;
			frame_size = (size * 8 * enc->sample_rate) / enc->bit_rate;
		}
	} else {
		frame_size = enc->frame_size;
	}
	return frame_size;
}

void ff_interleave_add_packet(AVFormatContext *s, AVPacket *pkt,
							  int (*compare)(AVFormatContext *, AVPacket *, AVPacket *))
{
	AVPacketList **next_point, *this_pktl;

	this_pktl = av_mallocz(sizeof(AVPacketList));
	this_pktl->pkt= *pkt;
	if(pkt->destruct == av_destruct_packet)
		pkt->destruct= NULL; // not shared -> must keep original from being freed
	else
		av_dup_packet(&this_pktl->pkt);  //shared -> must dup

	next_point = &s->packet_buffer;
	while(*next_point){
		if(compare(s, &(*next_point)->pkt, pkt))
			break;
		next_point= &(*next_point)->next;
	}
	this_pktl->next= *next_point;
	*next_point= this_pktl;
}

int ff_interleave_compare_dts(AVFormatContext *s, AVPacket *next, AVPacket *pkt)
{
	AVStream *st = s->streams[ pkt ->stream_index];
	AVStream *st2= s->streams[ next->stream_index];
	int64_t left = st2->time_base.num * (int64_t)st ->time_base.den;
	int64_t right= st ->time_base.num * (int64_t)st2->time_base.den;

	if (pkt->dts == AV_NOPTS_VALUE)
		return 0;

	return next->dts * left > pkt->dts * right; //FIXME this can overflow
}

int av_interleave_packet_per_dts(AVFormatContext *s, AVPacket *out, AVPacket *pkt, int flush){
	AVPacketList *pktl;
	int stream_count=0;
	int streams[MAX_STREAMS];

	if(pkt){
		ff_interleave_add_packet(s, pkt, ff_interleave_compare_dts);
	}

	memset(streams, 0, sizeof(streams));
	pktl= s->packet_buffer;
	while(pktl){
		//av_log(s, AV_LOG_DEBUG, "show st:%d dts:%"PRId64"\n", pktl->pkt.stream_index, pktl->pkt.dts);
		if(streams[ pktl->pkt.stream_index ] == 0)
			stream_count++;
		streams[ pktl->pkt.stream_index ]++;
		pktl= pktl->next;
	}

	if(stream_count && (s->nb_streams == stream_count || flush)){
		pktl= s->packet_buffer;
		*out= pktl->pkt;

		s->packet_buffer= pktl->next;
		av_freep(&pktl);
		return 1;
	}else{
		av_init_packet(out);
		return 0;
	}
}

/**
* Return the frame duration in seconds. Return 0 if not available.
*/
static void compute_frame_duration(int *pnum, int *pden, AVStream *st,
								   AVCodecParserContext *pc, AVPacket *pkt)
{
	int frame_size;

	*pnum = 0;
	*pden = 0;
	switch(st->codec->codec_type) {
	case CODEC_TYPE_VIDEO:
		if(st->time_base.num*1000LL > st->time_base.den){
			*pnum = st->time_base.num;
			*pden = st->time_base.den;
		}else if(st->codec->time_base.num*1000LL > st->codec->time_base.den){
			*pnum = st->codec->time_base.num;
			*pden = st->codec->time_base.den;
			if (pc && pc->repeat_pict) {
				*pnum = (*pnum) * (1 + pc->repeat_pict);
			}
		}
		break;
	case CODEC_TYPE_AUDIO:
		frame_size = get_audio_frame_size(st->codec, pkt->size);
		if (frame_size < 0)
			break;
		*pnum = frame_size;
		*pden = st->codec->sample_rate;
		break;
	default:
		break;
	}
}

static int is_intra_only(AVCodecContext *enc){
	if(enc->codec_type == CODEC_TYPE_AUDIO){
		return 1;
	}else if(enc->codec_type == CODEC_TYPE_VIDEO){
		switch(enc->codec_id){
		case CODEC_ID_MJPEG:
		case CODEC_ID_MJPEGB:
		case CODEC_ID_LJPEG:
		case CODEC_ID_RAWVIDEO:
		case CODEC_ID_DVVIDEO:
		case CODEC_ID_HUFFYUV:
		case CODEC_ID_FFVHUFF:
		case CODEC_ID_ASV1:
		case CODEC_ID_ASV2:
		case CODEC_ID_VCR1:
//		case CODEC_ID_DNXHD:
//		case CODEC_ID_JPEG2000:
			return 1;
		default: break;
		}
	}
	return 0;
}

//FIXME merge with compute_pkt_fields
static int compute_pkt_fields2(AVStream *st, AVPacket *pkt){
	int delay = FFMAX(st->codec->has_b_frames, !!st->codec->max_b_frames);
	int num, den, frame_size, i;

	//    av_log(st->codec, AV_LOG_DEBUG, "av_write_frame: pts:%"PRId64" dts:%"PRId64" cur_dts:%"PRId64" b:%d size:%d st:%d\n", pkt->pts, pkt->dts, st->cur_dts, delay, pkt->size, pkt->stream_index);

	/*    if(pkt->pts == AV_NOPTS_VALUE && pkt->dts == AV_NOPTS_VALUE)
	return -1;*/

	/* duration field */
	if (pkt->duration == 0) {
		compute_frame_duration(&num, &den, st, NULL, pkt);
		if (den && num) {
			pkt->duration = av_rescale(1, num * (int64_t)st->time_base.den * st->codec->ticks_per_frame, den * (int64_t)st->time_base.num);
		}
	}

	if(pkt->pts == AV_NOPTS_VALUE && pkt->dts != AV_NOPTS_VALUE && delay==0)
		pkt->pts= pkt->dts;

	//XXX/FIXME this is a temporary hack until all encoders output pts
	if((pkt->pts == 0 || pkt->pts == AV_NOPTS_VALUE) && pkt->dts == AV_NOPTS_VALUE && !delay){
		pkt->dts=
			//        pkt->pts= st->cur_dts;
			pkt->pts= st->pts.val;
	}

	//calculate dts from pts
	if(pkt->pts != AV_NOPTS_VALUE && pkt->dts == AV_NOPTS_VALUE && delay <= MAX_REORDER_DELAY){
		st->pts_buffer[0]= pkt->pts;
		for(i=1; i<delay+1 && st->pts_buffer[i] == AV_NOPTS_VALUE; i++)
			st->pts_buffer[i]= (i-delay-1) * pkt->duration;
		for(i=0; i<delay && st->pts_buffer[i] > st->pts_buffer[i+1]; i++)
			FFSWAP(int64_t, st->pts_buffer[i], st->pts_buffer[i+1]);

		pkt->dts= st->pts_buffer[0];
	}

	if(st->cur_dts && st->cur_dts != AV_NOPTS_VALUE && st->cur_dts >= pkt->dts){
		av_log(st->codec, AV_LOG_ERROR, "error, non monotone timestamps %"PRId64" >= %"PRId64"\n", st->cur_dts, pkt->dts);
		return -1;
	}
	if(pkt->dts != AV_NOPTS_VALUE && pkt->pts != AV_NOPTS_VALUE && pkt->pts < pkt->dts){
		av_log(st->codec, AV_LOG_ERROR, "error, pts < dts\n");
		return -1;
	}

	//    av_log(NULL, AV_LOG_DEBUG, "av_write_frame: pts2:%"PRId64" dts2:%"PRId64"\n", pkt->pts, pkt->dts);
	st->cur_dts= pkt->dts;
	st->pts.val= pkt->dts;

	/* update pts */
	switch (st->codec->codec_type) {
	case CODEC_TYPE_AUDIO:
		frame_size = get_audio_frame_size(st->codec, pkt->size);

		/* HACK/FIXME, we skip the initial 0 size packets as they are most
		likely equal to the encoder delay, but it would be better if we
		had the real timestamps from the encoder */
		if (frame_size >= 0 && (pkt->size || st->pts.num!=st->pts.den>>1 || st->pts.val)) {
			av_frac_add(&st->pts, (int64_t)st->time_base.den * frame_size);
		}
		break;
	case CODEC_TYPE_VIDEO:
		av_frac_add(&st->pts, (int64_t)st->time_base.den * st->codec->time_base.num);
		break;
	default:
		break;
	}
	return 0;
}


int av_write_frame(AVFormatContext *s, AVPacket *pkt)
{
	int ret = compute_pkt_fields2(s->streams[pkt->stream_index], pkt);

	if(ret<0 && !(s->oformat->flags & AVFMT_NOTIMESTAMPS))
		return ret;

	ret= s->oformat->write_packet(s, pkt);
	if(!ret)
		ret= url_ferror(s->pb);
	return ret;
}

/**
* Interleaves an AVPacket correctly so it can be muxed.
* @param out the interleaved packet will be output here
* @param in the input packet
* @param flush 1 if no further packets are available as input and all
*              remaining packets should be output
* @return 1 if a packet was output, 0 if no packet could be output,
*         < 0 if an error occurred
*/
static int av_interleave_packet(AVFormatContext *s, AVPacket *out, AVPacket *in, int flush){
	if(s->oformat->interleave_packet)
		return s->oformat->interleave_packet(s, out, in, flush);
	else
		return av_interleave_packet_per_dts(s, out, in, flush);
}

int av_write_trailer(AVFormatContext *s)
{
	int ret, i;

	for(;;){
		AVPacket pkt;
		ret= av_interleave_packet(s, &pkt, NULL, 1);
		if(ret<0) //FIXME cleanup needed for ret<0 ?
			goto fail;
		if(!ret)
			break;

		ret= s->oformat->write_packet(s, &pkt);

		av_free_packet(&pkt);

		if(ret<0)
			goto fail;
		if(url_ferror(s->pb))
			goto fail;
	}

//	av_log( s, AV_LOG_ERROR, "before write_trailer\n" );

	if(s->oformat->write_trailer)
		ret = s->oformat->write_trailer(s);
fail:
//	av_log( s, AV_LOG_ERROR, "begin fail!\n" );
	if(ret == 0)
		ret=url_ferror(s->pb);
	for(i=0;i<s->nb_streams;i++)
	{
		if( s->streams[i] )
			av_freep(&s->streams[i]->priv_data);
	}
	av_freep(&s->priv_data);
	return ret;
}

/*******************************************************/


#ifdef WRITE_TO_FILE

static AVPacket *add_to_pktbuf(AVPacketList **packet_buffer, AVPacket *pkt,
							   AVPacketList **plast_pktl){
								   AVPacketList *pktl = av_mallocz(sizeof(AVPacketList));
								   if (!pktl)
									   return NULL;

								   if (*packet_buffer)
									   (*plast_pktl)->next = pktl;
								   else
									   *packet_buffer = pktl;

								   /* add the packet in the buffered packet list */
								   *plast_pktl = pktl;
								   pktl->pkt= *pkt;
								   return &pktl->pkt;
}


int av_read_packet(AVFormatContext *s, AVPacket *pkt)
{
	int ret;
	AVStream *st;

	for(;;){
		AVPacketList *pktl = s->raw_packet_buffer;

		if (pktl) {
			*pkt = pktl->pkt;
			if(s->streams[pkt->stream_index]->codec->codec_id != CODEC_ID_PROBE){
				s->raw_packet_buffer = pktl->next;
				av_free(pktl);
				return 0;
			}
		}

		av_init_packet(pkt);
		ret= s->iformat->read_packet(s, pkt);
		if (ret < 0)
			return ret;
		st= s->streams[pkt->stream_index];

		switch(st->codec->codec_type){
		case CODEC_TYPE_VIDEO:
			if(s->video_codec_id)   st->codec->codec_id= s->video_codec_id;
			break;
		case CODEC_TYPE_AUDIO:
			if(s->audio_codec_id)   st->codec->codec_id= s->audio_codec_id;
			break;
		case CODEC_TYPE_SUBTITLE:
			if(s->subtitle_codec_id)st->codec->codec_id= s->subtitle_codec_id;
			break;
		}

		if(!pktl && st->codec->codec_id!=CODEC_ID_PROBE)
			return ret;

		add_to_pktbuf(&s->raw_packet_buffer, pkt, &s->raw_packet_buffer_end);

		if(st->codec->codec_id == CODEC_ID_PROBE){
			AVProbeData *pd = &st->probe_data;

			pd->buf = av_realloc(pd->buf, pd->buf_size+pkt->size+AVPROBE_PADDING_SIZE);
			memcpy(pd->buf+pd->buf_size, pkt->data, pkt->size);
			pd->buf_size += pkt->size;
			memset(pd->buf+pd->buf_size, 0, AVPROBE_PADDING_SIZE);

			if(av_log2(pd->buf_size) != av_log2(pd->buf_size - pkt->size)){
				set_codec_from_probe_data(st, pd, 1);
				if(st->codec->codec_id != CODEC_ID_PROBE){
					pd->buf_size=0;
					av_freep(&pd->buf);
				}
			}
		}
	}
}


static int av_read_frame_internal(AVFormatContext *s, AVPacket *pkt)
{
	AVStream *st;
	int len, ret, i;

	av_init_packet(pkt);

	for(;;) {
		/* select current input stream component */
		st = s->cur_st;
		if (st) {
			if (!st->need_parsing || !st->parser) {
				/* no parsing needed: we just output the packet as is */
				/* raw data support */
				*pkt = st->cur_pkt; st->cur_pkt.data= NULL;
				compute_pkt_fields(s, st, NULL, pkt);
				s->cur_st = NULL;
				break;
			} else if (st->cur_len > 0 && st->discard < AVDISCARD_ALL) {
				len = av_parser_parse(st->parser, st->codec, &pkt->data, &pkt->size,
					st->cur_ptr, st->cur_len,
					st->cur_pkt.pts, st->cur_pkt.dts);
				st->cur_pkt.pts = AV_NOPTS_VALUE;
				st->cur_pkt.dts = AV_NOPTS_VALUE;
				/* increment read pointer */
				st->cur_ptr += len;
				st->cur_len -= len;

				/* return packet if any */
				if (pkt->size) {
					pkt->pos = st->cur_pkt.pos;              // Isn't quite accurate but close.
got_packet:
					pkt->duration = 0;
					pkt->stream_index = st->index;
					pkt->pts = st->parser->pts;
					pkt->dts = st->parser->dts;
					pkt->destruct = av_destruct_packet_nofree;
					compute_pkt_fields(s, st, st->parser, pkt);

					if((s->iformat->flags & AVFMT_GENERIC_INDEX) && pkt->flags & PKT_FLAG_KEY){
						ff_reduce_index(s, st->index);
						av_add_index_entry(st, st->parser->frame_offset, pkt->dts,
							0, 0, AVINDEX_KEYFRAME);
					}

					break;
				}
			} else {
				/* free packet */
				av_free_packet(&st->cur_pkt);
				s->cur_st = NULL;
			}
		} else {
			AVPacket cur_pkt;
			/* read next packet */
			ret = av_read_packet(s, &cur_pkt);
			if (ret < 0) {
				if (ret == AVERROR(EAGAIN))
					return ret;
				/* return the last frames, if any */
				for(i = 0; i < s->nb_streams; i++) {
					st = s->streams[i];
					if (st->parser && st->need_parsing) {
						av_parser_parse(st->parser, st->codec,
							&pkt->data, &pkt->size,
							NULL, 0,
							AV_NOPTS_VALUE, AV_NOPTS_VALUE);
						if (pkt->size)
							goto got_packet;
					}
				}
				/* no more packets: really terminate parsing */
				return ret;
			}
			st = s->streams[cur_pkt.stream_index];
			st->cur_pkt= cur_pkt;

			if(st->cur_pkt.pts != AV_NOPTS_VALUE &&
				st->cur_pkt.dts != AV_NOPTS_VALUE &&
				st->cur_pkt.pts < st->cur_pkt.dts){
					av_log(s, AV_LOG_WARNING, "Invalid timestamps stream=%d, pts=%"PRId64", dts=%"PRId64", size=%d\n",
						st->cur_pkt.stream_index,
						st->cur_pkt.pts,
						st->cur_pkt.dts,
						st->cur_pkt.size);
					//                av_free_packet(&st->cur_pkt);
					//                return -1;
			}

			if(s->debug & FF_FDEBUG_TS)
				av_log(s, AV_LOG_DEBUG, "av_read_packet stream=%d, pts=%"PRId64", dts=%"PRId64", size=%d,  flags=%d\n",
				st->cur_pkt.stream_index,
				st->cur_pkt.pts,
				st->cur_pkt.dts,
				st->cur_pkt.size,
				st->cur_pkt.flags);

			s->cur_st = st;
			st->cur_ptr = st->cur_pkt.data;
			st->cur_len = st->cur_pkt.size;
			if (st->need_parsing && !st->parser) {
				st->parser = av_parser_init(st->codec->codec_id);
				if (!st->parser) {
					/* no parser available: just output the raw packets */
					st->need_parsing = AVSTREAM_PARSE_NONE;
				}else if(st->need_parsing == AVSTREAM_PARSE_HEADERS){
					st->parser->flags |= PARSER_FLAG_COMPLETE_FRAMES;
				}
				if(st->parser && (s->iformat->flags & AVFMT_GENERIC_INDEX)){
					st->parser->frame_offset=
						st->parser->cur_offset= st->cur_pkt.pos;
				}
			}
		}
	}
	if(s->debug & FF_FDEBUG_TS)
		av_log(s, AV_LOG_DEBUG, "av_read_frame_internal stream=%d, pts=%"PRId64", dts=%"PRId64", size=%d, flags=%d\n",
		pkt->stream_index,
		pkt->pts,
		pkt->dts,
		pkt->size,
		pkt->flags);

	return 0;
}


int av_read_frame(AVFormatContext *s, AVPacket *pkt)
{
	AVPacketList *pktl;
	int eof=0;
	const int genpts= s->flags & AVFMT_FLAG_GENPTS;

	for(;;){
		pktl = s->packet_buffer;
		if (pktl) {
			AVPacket *next_pkt= &pktl->pkt;

			if(genpts && next_pkt->dts != AV_NOPTS_VALUE){
				while(pktl && next_pkt->pts == AV_NOPTS_VALUE){
					if(   pktl->pkt.stream_index == next_pkt->stream_index
						&& next_pkt->dts < pktl->pkt.dts
						&& pktl->pkt.pts != pktl->pkt.dts //not b frame
						/*&& pktl->pkt.dts != AV_NOPTS_VALUE*/){
							next_pkt->pts= pktl->pkt.dts;
					}
					pktl= pktl->next;
				}
				pktl = s->packet_buffer;
			}

			if(   next_pkt->pts != AV_NOPTS_VALUE
				|| next_pkt->dts == AV_NOPTS_VALUE
				|| !genpts || eof){
					/* read packet from packet buffer, if there is data */
					*pkt = *next_pkt;
					s->packet_buffer = pktl->next;
					av_free(pktl);
					return 0;
			}
		}
		if(genpts){
			int ret= av_read_frame_internal(s, pkt);
			if(ret<0){
				if(pktl && ret != AVERROR(EAGAIN)){
					eof=1;
					continue;
				}else
					return ret;
			}

			if(av_dup_packet(add_to_pktbuf(&s->packet_buffer, pkt,
				&s->packet_buffer_end)) < 0)
				return AVERROR(ENOMEM);
		}else{
			assert(!s->packet_buffer);
			return av_read_frame_internal(s, pkt);
		}
	}
}

/* absolute maximum size we read until we abort */
#define MAX_READ_SIZE        5000000

#define MAX_STD_TIMEBASES (60*12+5)

int av_find_stream_info(AVFormatContext *ic)
{
	int i, count, ret, read_size, j;
	AVStream *st;
	AVPacket pkt1, *pkt;
	int64_t last_dts[MAX_STREAMS];
	int64_t duration_gcd[MAX_STREAMS]={0};
	int duration_count[MAX_STREAMS]={0};
	double (*duration_error)[MAX_STD_TIMEBASES];
	int64_t old_offset = url_ftell(ic->pb);
	int64_t codec_info_duration[MAX_STREAMS]={0};
	int codec_info_nb_frames[MAX_STREAMS]={0};


	duration_error = av_mallocz(MAX_STREAMS * sizeof(*duration_error));
	if (!duration_error) return AVERROR(ENOMEM);

	for(i=0;i<ic->nb_streams;i++) {
		st = ic->streams[i];
		if(st->codec->codec_type == CODEC_TYPE_VIDEO){
			/*            if(!st->time_base.num)
			st->time_base= */
			if(!st->codec->time_base.num)
				st->codec->time_base= st->time_base;
		}
		//only for the split stuff
		if (!st->parser) {
			st->parser = av_parser_init(st->codec->codec_id);
			if(st->need_parsing == AVSTREAM_PARSE_HEADERS && st->parser){
				st->parser->flags |= PARSER_FLAG_COMPLETE_FRAMES;
			}
		}
	}

	for(i=0;i<MAX_STREAMS;i++){
		last_dts[i]= AV_NOPTS_VALUE;
	}

	count = 0;
	read_size = 0;
	for(;;) {
		if(url_interrupt_cb()){
			ret= AVERROR(EINTR);
			break;
		}

		/* check if one codec still needs to be handled */
		for(i=0;i<ic->nb_streams;i++) {
			st = ic->streams[i];
			if (!has_codec_parameters(st->codec))
				break;
			/* variable fps and no guess at the real fps */
			if(   tb_unreliable(st->codec)
				&& duration_count[i]<20 && st->codec->codec_type == CODEC_TYPE_VIDEO)
				break;
			if(st->parser && st->parser->parser->split && !st->codec->extradata)
				break;
			if(st->first_dts == AV_NOPTS_VALUE)
				break;
		}
		if (i == ic->nb_streams) {
			/* NOTE: if the format has no header, then we need to read
			some packets to get most of the streams, so we cannot
			stop here */
			if (!(ic->ctx_flags & AVFMTCTX_NOHEADER)) {
				/* if we found the info for all the codecs, we can stop */
				ret = count;
				break;
			}
		}
		/* we did not get all the codec info, but we read too much data */
		if (read_size >= MAX_READ_SIZE) {
			ret = count;
			break;
		}

		/* NOTE: a new stream can be added there if no header in file
		(AVFMTCTX_NOHEADER) */
		ret = av_read_frame_internal(ic, &pkt1);
		if (ret < 0) {
			/* EOF or error */
			ret = -1; /* we could not have all the codec parameters before EOF */
			for(i=0;i<ic->nb_streams;i++) {
				st = ic->streams[i];
				if (!has_codec_parameters(st->codec)){
					char buf[256];
					avcodec_string(buf, sizeof(buf), st->codec, 0);
					av_log(ic, AV_LOG_INFO, "Could not find codec parameters (%s)\n", buf);
				} else {
					ret = 0;
				}
			}
			break;
		}

		pkt= add_to_pktbuf(&ic->packet_buffer, &pkt1, &ic->packet_buffer_end);
		if(av_dup_packet(pkt) < 0) {
			av_free(duration_error);
			return AVERROR(ENOMEM);
		}

		read_size += pkt->size;

		st = ic->streams[pkt->stream_index];
		if(codec_info_nb_frames[st->index]>1)
			codec_info_duration[st->index] += pkt->duration;
		if (pkt->duration != 0)
			codec_info_nb_frames[st->index]++;

		{
			int index= pkt->stream_index;
			int64_t last= last_dts[index];
			int64_t duration= pkt->dts - last;

			if(pkt->dts != AV_NOPTS_VALUE && last != AV_NOPTS_VALUE && duration>0){
				double dur= duration * av_q2d(st->time_base);

				//                if(st->codec->codec_type == CODEC_TYPE_VIDEO)
				//                    av_log(NULL, AV_LOG_ERROR, "%f\n", dur);
				if(duration_count[index] < 2)
					memset(duration_error[index], 0, sizeof(*duration_error));
				for(i=1; i<MAX_STD_TIMEBASES; i++){
					int framerate= get_std_framerate(i);
					int ticks= lrintf(dur*framerate/(1001*12));
					double error= dur - ticks*1001*12/(double)framerate;
					duration_error[index][i] += error*error;
				}
				duration_count[index]++;
				// ignore the first 4 values, they might have some random jitter
				if (duration_count[index] > 3)
					duration_gcd[index] = av_gcd(duration_gcd[index], duration);
			}
			if(last == AV_NOPTS_VALUE || duration_count[index]<=1)
				last_dts[pkt->stream_index]= pkt->dts;
		}
		if(st->parser && st->parser->parser->split && !st->codec->extradata){
			int i= st->parser->parser->split(st->codec, pkt->data, pkt->size);
			if(i){
				st->codec->extradata_size= i;
				st->codec->extradata= av_malloc(st->codec->extradata_size + FF_INPUT_BUFFER_PADDING_SIZE);
				memcpy(st->codec->extradata, pkt->data, st->codec->extradata_size);
				memset( (char*)st->codec->extradata + i, 0, FF_INPUT_BUFFER_PADDING_SIZE);
			}
		}

		/* if still no information, we try to open the codec and to
		decompress the frame. We try to avoid that in most cases as
		it takes longer and uses more memory. For MPEG-4, we need to
		decompress for QuickTime. */
		if (!has_codec_parameters(st->codec) /*&&
											 (st->codec->codec_id == CODEC_ID_FLV1 ||
											 st->codec->codec_id == CODEC_ID_H264 ||
											 st->codec->codec_id == CODEC_ID_H263 ||
											 st->codec->codec_id == CODEC_ID_H261 ||
											 st->codec->codec_id == CODEC_ID_VORBIS ||
											 st->codec->codec_id == CODEC_ID_MJPEG ||
											 st->codec->codec_id == CODEC_ID_PNG ||
											 st->codec->codec_id == CODEC_ID_PAM ||
											 st->codec->codec_id == CODEC_ID_PGM ||
											 st->codec->codec_id == CODEC_ID_PGMYUV ||
											 st->codec->codec_id == CODEC_ID_PBM ||
											 st->codec->codec_id == CODEC_ID_PPM ||
											 st->codec->codec_id == CODEC_ID_SHORTEN ||
											 (st->codec->codec_id == CODEC_ID_MPEG4 && !st->need_parsing))*/)
											 try_decode_frame(st, pkt->data, pkt->size);

		if (st->time_base.den > 0 && av_rescale_q(codec_info_duration[st->index], st->time_base, AV_TIME_BASE_Q) >= ic->max_analyze_duration) 
		{
			break;
		}
		count++;
	}

	// close codecs which were opened in try_decode_frame()
	for(i=0;i<ic->nb_streams;i++) {
		st = ic->streams[i];
		if(st->codec->codec)
			avcodec_close(st->codec);
	}
	for(i=0;i<ic->nb_streams;i++) {
		st = ic->streams[i];
		if (st->codec->codec_type == CODEC_TYPE_VIDEO) {
			if(st->codec->codec_id == CODEC_ID_RAWVIDEO && !st->codec->codec_tag && !st->codec->bits_per_coded_sample)
				st->codec->codec_tag= avcodec_pix_fmt_to_codec_tag(st->codec->pix_fmt);

			// the check for tb_unreliable() is not completely correct, since this is not about handling
			// a unreliable/inexact time base, but a time base that is finer than necessary, as e.g.
			// ipmovie.c produces.
			if (tb_unreliable(st->codec) && duration_count[i] > 15 && duration_gcd[i] > 1)
				av_reduce(&st->r_frame_rate.num, &st->r_frame_rate.den, st->time_base.den, st->time_base.num * duration_gcd[i], INT_MAX);
			if(duration_count[i]
			&& tb_unreliable(st->codec) /*&&
										//FIXME we should not special-case MPEG-2, but this needs testing with non-MPEG-2 ...
										st->time_base.num*duration_sum[i]/duration_count[i]*101LL > st->time_base.den*/){
											int num = 0;
											double best_error= 2*av_q2d(st->time_base);
											best_error= best_error*best_error*duration_count[i]*1000*12*30;

											for(j=1; j<MAX_STD_TIMEBASES; j++){
												double error= duration_error[i][j] * get_std_framerate(j);
												//                    if(st->codec->codec_type == CODEC_TYPE_VIDEO)
												//                        av_log(NULL, AV_LOG_ERROR, "%f %f\n", get_std_framerate(j) / 12.0/1001, error);
												if(error < best_error){
													best_error= error;
													num = get_std_framerate(j);
												}
											}
											// do not increase frame rate by more than 1 % in order to match a standard rate.
											if (num && (!st->r_frame_rate.num || (double)num/(12*1001) < 1.01 * av_q2d(st->r_frame_rate)))
												av_reduce(&st->r_frame_rate.num, &st->r_frame_rate.den, num, 12*1001, INT_MAX);
			}

			if (!st->r_frame_rate.num){
				if(    st->codec->time_base.den * (int64_t)st->time_base.num
					<= st->codec->time_base.num * st->codec->ticks_per_frame * (int64_t)st->time_base.den){
						st->r_frame_rate.num = st->codec->time_base.den;
						st->r_frame_rate.den = st->codec->time_base.num * st->codec->ticks_per_frame;
				}else{
					st->r_frame_rate.num = st->time_base.den;
					st->r_frame_rate.den = st->time_base.num;
				}
			}
		}else if(st->codec->codec_type == CODEC_TYPE_AUDIO) {
			if(!st->codec->bits_per_coded_sample)
				st->codec->bits_per_coded_sample= av_get_bits_per_sample(st->codec->codec_id);
		}
	}

	av_estimate_timings(ic, old_offset);

	compute_chapters_end(ic);

#if 0
	/* correct DTS for B-frame streams with no timestamps */
	for(i=0;i<ic->nb_streams;i++) {
		st = ic->streams[i];
		if (st->codec->codec_type == CODEC_TYPE_VIDEO) {
			if(b-frames){
				ppktl = &ic->packet_buffer;
				while(ppkt1){
					if(ppkt1->stream_index != i)
						continue;
					if(ppkt1->pkt->dts < 0)
						break;
					if(ppkt1->pkt->pts != AV_NOPTS_VALUE)
						break;
					ppkt1->pkt->dts -= delta;
					ppkt1= ppkt1->next;
				}
				if(ppkt1)
					continue;
				st->cur_dts -= delta;
			}
		}
	}
#endif

	av_free(duration_error);

	return ret;
}

/** size of probe buffer, for guessing file type from file contents */
#define PROBE_BUF_MIN 2048
#define PROBE_BUF_MAX (1<<20)

int av_open_input_file(AVFormatContext **ic_ptr, const char *filename,
					   AVInputFormat *fmt,
					   int buf_size,
					   AVFormatParameters *ap)
{
	int err, probe_size;
	AVProbeData probe_data, *pd = &probe_data;
	ByteIOContext *pb = NULL;

	pd->filename = "";
	if (filename)
		pd->filename = filename;
	pd->buf = NULL;
	pd->buf_size = 0;

	if (!fmt) {
		/* guess format if no file can be opened */
		fmt = av_probe_input_format(pd, 0);
	}

	/* Do not open file if the format does not need it. XXX: specific
	hack needed to handle RTSP/TCP */
	if (!fmt || !(fmt->flags & AVFMT_NOFILE)) {
		/* if no file needed do not try to open one */
		if ((err=url_fopen(&pb, filename, URL_RDONLY)) < 0) {
			goto fail;
		}
		if (buf_size > 0) {
			url_setbufsize(pb, buf_size);
		}

		for(probe_size= PROBE_BUF_MIN; probe_size<=PROBE_BUF_MAX && !fmt; probe_size<<=1){
			int score= probe_size < PROBE_BUF_MAX ? AVPROBE_SCORE_MAX/4 : 0;
			/* read probe data */
			pd->buf= av_realloc(pd->buf, probe_size + AVPROBE_PADDING_SIZE);
			pd->buf_size = get_buffer(pb, pd->buf, probe_size);
			memset(pd->buf+pd->buf_size, 0, AVPROBE_PADDING_SIZE);
			if (url_fseek(pb, 0, SEEK_SET) < 0) {
				url_fclose(pb);
				if (url_fopen(&pb, filename, URL_RDONLY) < 0) {
					pb = NULL;
					err = AVERROR(EIO);
					goto fail;
				}
			}
			/* guess file format */
			fmt = av_probe_input_format2(pd, 1, &score);
		}
		av_freep(&pd->buf);
	}

	/* if still no format found, error */
	if (!fmt) {
		err = AVERROR_NOFMT;
		goto fail;
	}

	/* check filename in case an image number is expected */
	if (fmt->flags & AVFMT_NEEDNUMBER) {
		if (!av_filename_number_test(filename)) {
			err = AVERROR_NUMEXPECTED;
			goto fail;
		}
	}
	err = av_open_input_stream(ic_ptr, pb, filename, fmt, ap);
	if (err)
		goto fail;
	return 0;
fail:
	av_freep(&pd->buf);
	if (pb)
		url_fclose(pb);
	*ic_ptr = NULL;
	return err;

}

#endif