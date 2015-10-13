# Symbian开发 #

记录Symbian开发学习中的点点滴滴。


## 工具 ##
帮助可以搜索。

## 编码 ##
> ### 命名 trailing name。 ###

xxxxLC = xxx会 leave 会 create。

xxxxL = xxx会leave。

## 原理 ##
> ### CleanupStack ###
设计目的：在异常的时候（leave）释放资源。
An object can never be orphaned.
太多的地方用stack维护对象指针。


> ### 程序框架。 (Application Framework ) ###
> > #### Uikon ####
2部分：
      1. 应用程序架构框架（Application Architecture Framework )
      1. 界面控件框架    ( UI Control Framework )
5个概念
      1. Application - CEikApplication
      1. Document - CEikDocument
      1. AppUI - CEikAppUi
      1. UI Environment - CEikonEnv
      1. Utilities - EikFileUtils

  * Application
The application class defines properties of the application, such as UID and caption, and creates a new document.
The application base class is provided by CEikApplication.
  * Document

The document class represents the data model for the application. In file-based applications, it stores and restores the application's data. It handles requests to edit a document by creating an app UI.
The document base class is provided by CEikDocument.
  * App UI

The app UI is the central user interface class. It creates and owns controls to display the application data, and centralises handling of command input from standard controls such as menus and toolbars.
The app UI base class is provided by CEikAppUi, which can be customised by using the resource structure EIK\_APP\_INFO.
  * UI Environment

The UI Environment provides a large number of assorted UI functionality, particularly simple access to information and query dialogs, and access to standard system UI resources (bitmaps and fonts). Every object in a GUI application can access a UI Environment (through CEikonEnv::Static()), as a pointer to it is stored in thread-local storage.
The UI Environment is provided by CEikonEnv.
  * Utilities

Utility classes provide easy access for applications to frequently-used functionality. Notable are EikFileUtils for file access, and EikResourceUtils for resource access.


> ### view architecture ###
> Each view has its own control stack. Each view’s container and controls are created when it is called forward, and destroyed when another view of the same application is called forward.
DoActivateL 创建form对象。
DoDeactivate delete form对象。