#ifndef CONSTANTS_H
#define CONSTANTS_H


// Default Values
#define MAX_NUMBER_OF_FILE_THREADS                          10


// Application Info

#define DEFAULT_APPLICATION_NAME                            "MaxCommander"
#define DEFAULT_ORGANIZATION_NAME                           "MySoft"
#define DEFAULT_ORGANIZATION_DOMAIN                         "mysoft.com"


#define DEFAULT_WORKER_HOST_NAME                            "localhost"


// Settings Groups

#define SETTINGS_GROUP_MAIN                                 "main"
#define SETTINGS_GROUP_LEFTPANEL                            "leftpanel"
#define SETTINGS_GROUP_RIGHTPANEL                           "rightpanel"
#define SETTINGS_GROUP_PANEL_COMMON                         "panelCommon"
#define SETTINGS_GROUP_UI                                   "ui"
#define SETTINGS_GROUP_VIEWER                               "viewer"
#define SETTINGS_GROUP_APPS                                 "apps"
#define SETTINGS_GROUP_SEARCH                               "search"


// Settings Keys

// Main

#define SETTINGS_KEY_MAIN_EXEC_PATH                         SETTINGS_GROUP_MAIN"/execpath"

#define SETTINGS_KEY_MAIN_MAXIMIZED                         SETTINGS_GROUP_MAIN"/maximized"

#define SETTINGS_KEY_MAIN_POSX                              SETTINGS_GROUP_MAIN"/posx"
#define SETTINGS_KEY_MAIN_POSY                              SETTINGS_GROUP_MAIN"/posy"

#define SETTINGS_KEY_MAIN_WIDTH                             SETTINGS_GROUP_MAIN"/width"
#define SETTINGS_KEY_MAIN_HEIGHT                            SETTINGS_GROUP_MAIN"/height"

#define SETTINGS_KEY_MAIN_ACTIVEPANEL                       SETTINGS_GROUP_MAIN"/activePanel"


// UI

#define SETTINGS_KEY_MAIN_SPLITTER                          SETTINGS_GROUP_UI"/mainSplitter"

#define SETTINGS_KEY_SHOW_FUNCTION_KEYS                     SETTINGS_GROUP_UI"/showFunctionKeys"
#define SETTINGS_KEY_SHOW_DIR_HOT_KEYS                      SETTINGS_GROUP_UI"/showDirHotKeys"
#define SETTINGS_KEY_SHOW_DRIVE_BUTTONS                     SETTINGS_GROUP_UI"/showDriveButtons"

#define SETTINGS_KEY_THUMBS_WIDTH                           SETTINGS_GROUP_UI"/thumbsWidth"
#define SETTINGS_KEY_THUMBS_HEIGHT                          SETTINGS_GROUP_UI"/thumbsHeiight"

#define SETTINGS_KEY_FOCUSED_PANEL                          SETTINGS_GROUP_UI"/focusedPanel"

#define SETTINGS_KEY_CLOSE_WHEN_FINISHED                    SETTINGS_GROUP_UI"/closeWhenFinished"

#define SETTINGS_KEY_SHOW_FULL_FILE_SIZE                    SETTINGS_GROUP_UI"/showFullFileSize"

#define SETTINGS_KEY_PANEL_COLOR_TEXT                       SETTINGS_GROUP_UI"/textColor"
#define SETTINGS_KEY_PANEL_COLOR_TEXT_BG                    SETTINGS_GROUP_UI"/textBGColor"

#define SETTINGS_KEY_PANEL_COLOR_CURRENT                    SETTINGS_GROUP_UI"/currentColor"
#define SETTINGS_KEY_PANEL_COLOR_CURRENT_BG                 SETTINGS_GROUP_UI"/currentBGColor"

#define SETTINGS_KEY_PANEL_COLOR_SELECTED                   SETTINGS_GROUP_UI"/selectedColor"
#define SETTINGS_KEY_PANEL_COLOR_SELECTED_BG                SETTINGS_GROUP_UI"/selectedBGColor"

#define SETTINGS_KEY_PANEL_COLOR_CURRENT_SELECTED           SETTINGS_GROUP_UI"/currentSelectedColor"
#define SETTINGS_KEY_PANEL_COLOR_CURRENT_SELECTED_BG        SETTINGS_GROUP_UI"/currentSelectedBGColor"

#define SETTINGS_KEY_PANEL_COLOR_HIDDEN                     SETTINGS_GROUP_UI"/hiddenColor"
#define SETTINGS_KEY_PANEL_COLOR_HIDDEN_BG                  SETTINGS_GROUP_UI"/hiddenBGColor"

#define SETTINGS_KEY_PANEL_COLOR_LINK                       SETTINGS_GROUP_UI"/linkColor"
#define SETTINGS_KEY_PANEL_COLOR_LINK_BG                    SETTINGS_GROUP_UI"/linkBGColor"

#define SETTINGS_KEY_PANEL_FONT_NAME                        SETTINGS_GROUP_UI"/fontName"
#define SETTINGS_KEY_PANEL_FONT_SIZE                        SETTINGS_GROUP_UI"/fontSize"
#define SETTINGS_KEY_PANEL_FONT_BOLD                        SETTINGS_GROUP_UI"/fontBold"
#define SETTINGS_KEY_PANEL_FONT_ITALIC                      SETTINGS_GROUP_UI"/fontItalic"

#define SETTINGS_KEY_APPS_TERMINAL                          SETTINGS_GROUP_APPS"/terminal"
#define SETTINGS_KEY_APPS_VIEWER                            SETTINGS_GROUP_APPS"/viewer"
#define SETTINGS_KEY_APPS_EDITOR                            SETTINGS_GROUP_APPS"/editor"
#define SETTINGS_KEY_APPS_COMPARE                           SETTINGS_GROUP_APPS"/compare"
#define SETTINGS_KEY_APPS_PACKER                            SETTINGS_GROUP_APPS"/packer"
#define SETTINGS_KEY_APPS_UNPACKER                          SETTINGS_GROUP_APPS"/unpacker"




// Common

#define SETTINGS_KEY_SHOW_HIDDEN_FILES                      SETTINGS_GROUP_PANEL_COMMON"/showHidden"
#define SETTINGS_KEY_DIRFIRST                               SETTINGS_GROUP_PANEL_COMMON"/dirFirst"
#define SETTINGS_KEY_CASE_SENSITIVE                         SETTINGS_GROUP_PANEL_COMMON"/caseSensitive"
#define SETTINGS_KEY_SELECT_DIRS                            SETTINGS_GROUP_PANEL_COMMON"/selectDirs"

#define SETTINGS_KEY_FOLLOW_LINKS                           SETTINGS_GROUP_PANEL_COMMON"/followLinks"

#define SETTINGS_KEY_PANEL_COPY_HIDDEN_FILES                SETTINGS_GROUP_PANEL_COMMON"/copyHidden"

#define SETTINGS_KEY_PANEL_USE_DEFAULT_ICONS                SETTINGS_GROUP_UI"/defaultIcons"
#define SETTINGS_KEY_SHOW_FULL_SIZES                        SETTINGS_GROUP_UI"/showFullSizes"


// File Panels

#define DEFAULT_PANEL_NAME_LEFT                             "LEFT_PANEL"
#define DEFAULT_PANEL_NAME_RIGHT                            "RIGHT_PANEL"


// Panel Settings - Use With Panel Name!!

#define SETTINGS_KEY_PANEL_DIR                              "/dir"
#define SETTINGS_KEY_PANEL_SORTTYPE                         "/sortType"
#define SETTINGS_KEY_PANEL_REVERSE                          "/reverse"

#define SETTINGS_KEY_PANEL_COL_ENABLED_EXT                  "/columnExt"
#define SETTINGS_KEY_PANEL_COL_ENABLED_TYPE                 "/columnType"
#define SETTINGS_KEY_PANEL_COL_ENABLED_SIZE                 "/columnSize"
#define SETTINGS_KEY_PANEL_COL_ENABLED_DATE                 "/columnDate"
#define SETTINGS_KEY_PANEL_COL_ENABLED_OWNER                "/columnOwner"
#define SETTINGS_KEY_PANEL_COL_ENABLED_PERMS                "/columnPerms"
#define SETTINGS_KEY_PANEL_COL_ENABLED_ATTR                 "/columnAttr"

#define SETTINGS_KEY_PANEL_COL_WIDTH_EXT                    "/widthExt"
#define SETTINGS_KEY_PANEL_COL_WIDTH_TYPE                   "/widthType"
#define SETTINGS_KEY_PANEL_COL_WIDTH_SIZE                   "/widthSize"
#define SETTINGS_KEY_PANEL_COL_WIDTH_DATE                   "/widthDate"
#define SETTINGS_KEY_PANEL_COL_WIDTH_OWNER                  "/widthOwner"
#define SETTINGS_KEY_PANEL_COL_WIDTH_PERMS                  "/widthPerms"
#define SETTINGS_KEY_PANEL_COL_WIDTH_ATTR                   "/widthAttr"



// Viewer

#define SETTINGS_KEY_VIEWER_WIDTH                           SETTINGS_GROUP_VIEWER"/width"
#define SETTINGS_KEY_VIEWER_HEIGHT                          SETTINGS_GROUP_VIEWER"/height"

#define SETTINGS_KEY_VIEWER_WORDWRAP                        SETTINGS_GROUP_VIEWER"/wordwrap"
#define SETTINGS_KEY_VIEWER_FONTNAME                        SETTINGS_GROUP_VIEWER"/fontName"
#define SETTINGS_KEY_VIEWER_FONTSIZE                        SETTINGS_GROUP_VIEWER"/fontSize"
#define SETTINGS_KEY_VIEWER_FONTBOLD                        SETTINGS_GROUP_VIEWER"/fontBold"
#define SETTINGS_KEY_VIEWER_FONTITALIC                      SETTINGS_GROUP_VIEWER"/fontItalic"


#define DEFAULT_MIME_PREFIX_TEXT                            "text/"
#define DEFAULT_MIME_PREFIX_IMAGE                           "image/"
#define DEFAULT_MIME_PREFIX_AUDIO                           "audio/"
#define DEFAULT_MIME_PREFIX_VIDEO                           "video/"
#define DEFAULT_MIME_PREFIX_APP                             "application/"

#define DEFAULT_MIME_TEXT                                   "text"
#define DEFAULT_MIME_XML                                    "xml"
#define DEFAULT_MIME_SHELLSCRIPT                            "x-shellscript"
#define DEFAULT_MIME_JAVASCRIPT                             "javascript"
#define DEFAULT_MIME_SUBRIP                                 "x-subrip"


// Apps

#define SETTINGS_KEY_APPS_TERMINAL                          SETTINGS_GROUP_APPS"/terminal"
#define SETTINGS_KEY_APPS_VIEWER                            SETTINGS_GROUP_APPS"/viewer"
#define SETTINGS_KEY_APPS_EDITOR                            SETTINGS_GROUP_APPS"/editor"
#define SETTINGS_KEY_APPS_COMPARE                           SETTINGS_GROUP_APPS"/compare"
#define SETTINGS_KEY_APPS_PACKER                            SETTINGS_GROUP_APPS"/packer"
#define SETTINGS_KEY_APPS_UNPACKER                          SETTINGS_GROUP_APPS"/unpacker"


// Search

#define SETTINGS_KEY_SEARCH_CASE_SENSITIVE                  SETTINGS_GROUP_SEARCH"/caseSensitive"
#define SETTINGS_KEY_SEARCH_WHOLE_WORD                      SETTINGS_GROUP_SEARCH"/wholeWord"



// Default Settings


// Constants

// QML Context Properties
#define DEFAULT_MAIN_CONTROLLER_NAME                        "mainController"
#define DEFAULT_FILE_LIST_MODEL_NAME                        "fileListModel"
#define DEFAULT_IMAGE_VIEWER_CONTROLLER                     "viewerController"
#define DEFAULT_IMAGE_VIEWER_CONTENT                        "viewerContent"
#define DEFAULT_IMAGE_BROWSER                               "imageBrowser"
#define DEFAULT_CLIENT_LIST_MODEL_NAME                      "clientListModel"
#define DEFAULT_PREVIEW_CONTROLLER                          "previewController"
#define DEFAULT_SEARCH_RESULT_CONTROLLER                    "searchResultController"
#define DEFAULT_SEARCH_RESULT_MODEL                         "searchResultModel"
#define DEFAULT_GLOBAL_SETTINGS_CONTROLLER                  "globalSettings"
#define DEFAULT_DIR_HISTORY_LIST_CONTROLLER                 "dirHistoryListController"
#define DEFAULT_DIR_HISTORY_LIST_MODEL                      "dirHistoryListModel"



#define DEFAULT_ONE_KILO                                    1024

// Default Transparent Color
#define DEFAULT_TRANSPARENT_COLOR                           "#00000000"

#define DEFAULT_PANEL_FOCUSED_STYLE_SHEET                   "background-color: #11000077"
#define DEFAULT_PANEL_TRASPARENT_STYLE_SHEET                "background-color: #00000000"

#define DEFAULT_AVAILABLE_SPACE_FORMAT_STRING               "%1 files, %2 of %3 free"

#define DEFAULT_FILE_ICON_PROVIDER_ID                       "fileicon"
#define DEFAULT_AUDIO_TAG_PROVIDER_ID                       "audiotag"

// Bits Per Color Components
#define DEFAULT_BITS_PER_COMPONENTS                         8

// Default Icon Get Retry Count Max
#define DEFAULT_ICOM_GET_RETRY_COUNT_MAX                    5

// Default Root Shell Command Template
#define DEFAULT_ROOT_SHELL_COMMAND_TEMPLATE                 "echo \"%1\" | sudo -S \"%2\" &"

// Default PS Command Line To Check If File Serverr Is Running
#define DEFAULT_PS_COMMAND_CHECK_FILESERVER                 "ps x"

// Default File Server Launch Delay
#define DEFAULT_FILE_SERVER_LAUNCH_DELAY                    200

// Date Format String
#define DEFAULT_DATE_FORMAT_STRING                          "%1-%2-%3 %4:%5:%6"

// Default Icon Sizes
#define DEFAULT_ICON_WIDTH_16                               16
#define DEFAULT_ICON_HEIGHT_16                              16

#define DEFAULT_ICON_WIDTH_32                               32
#define DEFAULT_ICON_HEIGHT_32                              32

#define DEFAULT_ICON_WIDTH_64                               64
#define DEFAULT_ICON_HEIGHT_64                              64


#define DEFAULT_ONE_SEC                                     1000
#define DEFAULT_ANIM_FRAME_RATE                             30
#define DEFAULT_ANIM_PIECES_MAX                             3


#define DEFAULT_AUDIO_TAG_IMAGE_WIDTH                       256
#define DEFAULT_AUDIO_TAG_IMAGE_HEIGHT                      256


#define DEFAULT_STATUS_BAR_MESSAGE_TIMEOUT                  3000

// Progress Dialogs

#define DEFAULT_PROGRESS_DIALOG_COLUMN_WIDTH_OP             30
#define DEFAULT_PROGRESS_DIALOG_COLUMN_WIDTH_DONE           40
#define DEFAULT_PROGRESS_DIALOG_TEXT_MARGIN                 4
#define DEFAULT_PROGRESS_DIALOG_SCROLLBAR_WIDTH             10

#define DEFAULT_PROGRESS_DIALOG_SEMI_TRANSPARENCY           0.4


#define DEFAULT_CONFIRM_BUTTON_MINIMAL_WIDTH                80


// Icons
#define DEFAULT_ICON_PATH_OK                                ":/resources/images/check_256.png"
#define DEFAULT_ICON_PATH_CANCEL                            ":/resources/images/delete_256.png"
#define DEFAULT_ICON_PATH_PROGRESS                          ":/resources/images/synchronize_256.png"


// Exec Command
#define DEFAULT_EXEC_APP_SYSTEM_COMMAND_MAC_OSX             "open -a \"%1\" &"

// Exec App with Parameter
#define DEFAULT_EXEC_APP_SYSTEM_COMMAND_WITH_PARAM_MAC_OSX  "open -a %1 \"%2\" &"

// Open Files
#define DEFAULT_OPEN_FILE_SYSTEM_COMMAND_MAC_OSX            "open \"%1\" &"

// Default Min Height - Results
#define DEFAULT_FILE_SEARCH_DIALOG_MIN_HEIGHT_RESULTS       480
// Default Max Height - Results
#define DEFAULT_FILE_SEARCH_DIALOG_MAX_HEIGHT_RESULTS       1200

// Default Min Height - No Results
#define DEFAULT_FILE_SEARCH_DIALOG_MIN_HEIGHT_NO_RESULTS    190
// Default Max Height - No Results
#define DEFAULT_FILE_SEARCH_DIALOG_MAX_HEIGHT_NO_RESULTS    190

// File Search - File Name Pattern List
#define DEFAULT_FILE_SEARCH_FILE_NAME_PATTERN_LIST          ".searchfilename.list"
// File Search - Content Pattern List
#define DEFAULT_FILE_SEARCH_CONTENT_PATTERN_LIST            ".searchcontent.list"

// Max Numbers Of Patterns To Save
#define DEFAULT_FILE_SEARCH_MAX_PATTERNS                    16

// Suffix - .tag.gz
#define DEFAULT_SUFFIX_TAR_GZ                               "tar.gz"

// Default Icons
#define DEFAULT_FILE_ICON_FILE                              ":/resources/images/icons/default_file.png"
#define DEFAULT_FILE_ICON_DIR                               ":/resources/images/icons/default_folder_yellow.png"


// Default Permissions
#define DEFAULT_PERMISSIONS_TEXT                            "----------"

// File Select - File Name Pattern List
#define DEFAULT_FILE_SELECT_FILE_NAME_PATTERN_LIST          ".selectfilename.list"

#define DEFAULT_FILE_LIST_SIZE_BUNDLE                       "[BUNDLE]"
#define DEFAULT_FILE_LIST_SIZE_DIR                          "[DIR]"

#define DEFAULT_ICON_INFO                                   ":/resources/images/info.png"
#define DEFAULT_ICON_WARNING                                ":/resources/images/warning.png"
#define DEFAULT_ICON_ERROR                                  ":/resources/images/error.png"

#define DEFAULT_DIR_HISTORY_LIST_ITEMS_MAX                  16

#define DEFAULT_FILE_LIST_DIR_HSITORY_FILENAME              ".dirHistory%1.list"

#define DEFAULT_FILE_LIST_DIR_HISTORY_ITEM_HEIGHT           24
#define DEFAULT_FILE_LIST_DIR_HISTORY_RADIUS                8
#define DEFAULT_FILE_LIST_DIR_HISTORY_EMPTY_HEIGHT          46














// Localization Constants

#define DEFAULT_ERROR_TITLE_DELETE_FILE                     "File Deleting Error"
#define DEFAULT_ERROR_TITLE_COPY_FILE                       "File Copy Error"
#define DEFAULT_ERROR_TITLE_MOVE_FILE                       "File Rename/Move Error"
#define DEFAULT_ERROR_TITLE_SCAN_DIR                        "Dir Scanning Error"

#define DEFAULT_ERROR_TEXT_FILE_DOESNT_EXIST                "File Doesn't Exist:"

#define DEFAULT_TITLE_CONFIRMATION                          "Confirmation"
#define DEFAULT_TITLE_COPY_FILES                            "Copy File(s)"
#define DEFAULT_TITLE_MOVE_FILES                            "Move/Rename File(s)"

#define DEFAULT_CONFIRM_BUTTON_TEXT_YES                     "Yes"
#define DEFAULT_CONFIRM_BUTTON_TEXT_YESTOALL                "Yes to All"
#define DEFAULT_CONFIRM_BUTTON_TEXT_NO                      "No"
#define DEFAULT_CONFIRM_BUTTON_TEXT_NOTOALL                 "No to All"
#define DEFAULT_CONFIRM_BUTTON_TEXT_SKIP                    "Skip"
#define DEFAULT_CONFIRM_BUTTON_TEXT_SKIPALL                 "Skip All"
#define DEFAULT_CONFIRM_BUTTON_TEXT_RETRY                   "Retry"
#define DEFAULT_CONFIRM_BUTTON_TEXT_ASADMIN                 "As Admin"


#define DEFAULT_CONFIRM_TEXT_DIRECTORY_EXISTS               "Directory Exists:"
#define DEFAULT_CONFIRM_TEXT_DIRECTORY_NOT_EMPTY            "Directory: %1 Is Not Empty. Delete All?"
#define DEFAULT_CONFIRM_TEXT_DIRECTORY_EXISTS_MERGE         "Target Directory Exists, Merge?"
#define DEFAULT_CONFIRM_TEXT_TARGET_FILE_EXISTS             "Target File Exists: %1 Overwrite?"
#define DEFAULT_ERROR_TEXT_CANT_CREATE_DIRECTORY            "Can Not Create Directory:"
#define DEFAULT_ERROR_TEXT_CANT_DELETE_FILE                 "Can Not Delete File:"

#define DEFAULT_ERROR_TEXT_CANT_COPY_FILE                   "Can Not Copy File:"
#define DEFAULT_ERROR_TEXT_CANT_MOVE_FILE                   "Can Not Rename/Move File:"

#define DEFAULT_ERROR_TEXT_CANT_SCAN_DIR                    "Can Not Scan Directory:"

#define DEFAULT_ERROR_TEXT_CANT_DELETE_SOURCE               "Can Not Delete Source:"
#define DEFAULT_ERROR_TEXT_CANT_DELETE_TARGET               "Can Not Delete Target:"

#define DEFAULT_CONFIRM_TEXT_TARGET_DIR_DOESNT_EXIST        "Target Dir Doesn\'t Exist. Create?"

#define DEFAULT_PROGRESS_DIALOG_FILENAME_TEXT_TEMPLATE      "%1 @ %2 KB/s"


#define DEFAULT_TITLE_VIEWER                                "Viewer - "
#define DEFAULT_TITLE_EDITOR                                "Editor - "
#define DEFAULT_TITLE_EDITOR_NEW_FILE                       "Editor - New File"

#define DEFAULT_TITLE_SAVE_NEW_FILE                         "Save New File"
#define DEFAULT_TITLE_SAVE_FILE                             "Save File"

#define DEFAULT_TITLE_INFO                                  "Information"
#define DEFAULT_TITLE_WARNING                               "Warning"
#define DEFAULT_TITLE_ERROR                                 "Error"


#define DEFAULT_TEXT_WORD_WRAP_ON                           "Wrap On"
#define DEFAULT_TEXT_WORD_WRAP_OFF                          "Wrap Off"


#define DEFAULT_WARNING_TEXT_CANT_CREATE_NEW_FILE           "Could not Create New File!"
#define DEFAULT_WARNING_TEXT_UNSUPPORTED_FILE_FORMAT        "Unsupported File Format!"


#define DEFAULT_TITLE_FONT_SIZE                             "size: "

#define DEFAULT_BUTTON_TEXT_START                           "Start"

#define DEFAULT_LABEL_CURRENT_FILE_TITLE                    "Current File:"
#define DEFAULT_LABEL_CURRENT_FILE_TITLE_FINISHED           "Finished"


#define DEFAULT_TITLE_SELECT_LINK_TARGET                    "Select Link Target"


#define DEFAULT_TITLE_SELECT_FILES                          "Select Files"
#define DEFAULT_TITLE_DESELECT_FILES                        "Deselect Files"


#define DEFAULT_TITLE_FILE_MODIFIED                         "File Modified"
#define DEFAULT_CONFIRM_TEXT_SAVE_CHANGES                   "Save Changes?"

#define DEFAULT_TITLE_SELECT_FONT                           "Select Font"

#define DEFAULT_WARNING_TEXT_INVALID_SOURCE                 "Invalid Source"
#define DEFAULT_WARNING_TEXT_INVALID_TARGET                 "Invalid Target"

#define DEFAULT_FILE_PANEL_TITLE_SEARCH_RESULTS             "Search Results"


#define DEFAULT_HEADER_TITLE_OPERATION                      "Op"
#define DEFAULT_HEADER_TITLE_FILENAME                       "File Name"
#define DEFAULT_HEADER_TITLE_SOURCE                         "Source"
#define DEFAULT_HEADER_TITLE_TARGET                         "Target"
#define DEFAULT_HEADER_TITLE_DONE                           "Done"


#define DEFAULT_INFO_TEXT_NO_MATCH_FOUND                    "No Matching File Found"


#endif // CONSTANTS_H

