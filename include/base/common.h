// Copyright (c) 2017 The TAL Authors.
// All rights reserved.


#ifndef BASE_COMMON_H_
#define BASE_COMMON_H_

#include <list>
#include <vector>
#include <string.h>

/*录制视频相关*/
// 客户端到服务器
#define VIDEO_RECORD_END 0
#define VIDEO_RECORD_START 1

// 服务器到客户端
#define VIDEO_RECORD_SUCCESS 0
#define VIDEO_RECORD_FAIL 1

// config 文件相关
#define CONFIG_FILE_PATH "config"
#define CONFIG_FILE_NAME "config.ini"
#define UPDATE_FILE_NAME "fcupdate.ini"

#define RSA_BITS 2048
#define RSA_SERVER_PUBLICKEY_FILE "public.pem"
#define RSA_SERVER_PRIVATEEY_FILE "private.pem" //zgc add 20180412 temp
#define RSA_LOCAL_PRIVATEEY_FILE "local_private"
#define RSA_LOCAL_PUBLIC_FILE "local_public"

#define AES_KEY "jjSNlzedZzb!FUSVTRkpOWghR!0m9kSo"
#define AES_IV "R$0dLft3kQNVHOdm"

#define INI_FILE_SECTION_OSS "oss"
#define INI_FILE_SECTION_URL "url"
#define INI_FILE_SECTION_DEBUG "debug"
#define INI_FILE_SECTION_STATUS "status"
#define INI_FILE_SECTION_CONTROL "control"

// 记录下学生上一帧分数状态：down下降 up升高
#define STU_SCORE_STATUS_DOWN = "down"
#define STU_SCORE_STATUS_UP = "up"

// 上传文件类型
#define UPLOAD_FILE_TYPE_VIDEO 0
#define UPLOAD_FILE_TYPE_IMAGE 1

// 录制本地视频
#define LOCAL_VIDEO_STOP 0
#define LOCAL_VIDEO_START 1
#define LOCAL_VIDEO_PAUSE 2

#define DATA_REPORT_HOST "101.201.118.73"
#define DATA_REPORT_PORT 8080
#define DATA_REPORT_URL "/api/v1/data-report"
// URL相关
//#define URL_BACKEND_CAL "/api/dt/status-complete?cla_id=%s&cuc_num=%d"
//#define URL_BRING_BACK_HEART "/api/v1/heart/getCountStatus?class_id=%s&cuc_num=%d"
//#define URL_BRING_BACK_HEART_NEW "/api/v2/py/heart-get-count-status?live_id=%s&class_id=%s&cuc_num=%d"

//#define URL_IDMIS_UPDATE "/updatepcinfor/dev_id"
#define URL_VIDEO_PRE "http://ailabmirror-smallvideo.oss-cn-hangzhou.aliyuncs.com/"
#define DEFAULT_URL_PREFERFIX "http://image.fer.talbrain.com/"

#define LOG_FILE_DIR "C:\\mirror_log\\"
#define DUMP_FILE_DIR L"C:\\mirror_log\\"
#define VIDEO_FILE_DIR "C:\\mirror_log\\upload\\"
#define WATER_MARK_DIR "resource\\watermark\\watermark.jpg"
#define PROCESS_UPDATE "fcupdate.exe"
#define APPDATA_MIRROR L"Mirror"
// uuid注册表
#define UUID_REG_KEY L"SOFTWARE\\AILab\\Cryptography"
#define UUID_REG_SUBKEY L"uuid"
#define PRODUCT_REG_KEY L"SOFTWARE\\TAL\\Mirror"
#define PRODUCT_REG_VERSION L"Version"

// 渠道信息
enum CHANNEL_INFO {
    CHANNEL_START = 0,
    CHANNEL_DOUBLE_TEACHER,  // 双师
    CHANNEL_TOG,  // 公立校
    CHANNEL_DAHAI,
    CHANNEL_WEIDU,
    CHANNEL_END
};

// 上传的数据类型
enum CLIENT_TYPE {
    CLIENT_TYPE_START = 0,
    CLIENT_TYPE_CLIENT,
    CLIENT_TYPE_SERVER,
    CLIENT_TYPE_END,
};

// 日志等级
enum LOG_LEVEL {
    LOG_DEBUG = 0,
    LOG_NOTIFICATION,
    LOG_WARING,
    LOG_ERROR,
    LOG_FATAL,
};

// 和FT协议字段
enum CMD_ID {
    CMD_ID_START = 0,
    CMD_ID_STU_DATA = 1,              // 学生班级信息
    CMD_ID_CLASS_STATE = 2,           // 班级状态数据
    CMD_ID_CONFIG_DATA = 3,           // 初始化配置数据
    CMD_ID_VIDEO_RECORD = 4,          // 互动视频录制消息
    CMD_ID_CLASS_INFO = 5,            // 班级信息
    CMD_ID_STU_MAP = 6,               // 学生ID映射表
    CMD_ID_UPLOAD_VIDEO = 7,          // 是否上传小视频
    CMD_ID_NOTIFY_STATE = 8,          // 通知分析状态
    CMD_ID_RECORD_VIDEO_CONTROL = 9,  // 控制视频录制
    CMD_ID_FRAME_DATA,                // 每帧数据
    CMD_ID_CAMERA_STATUS,             // 摄像头状态
    CMD_ID_GALLERY_RESPONSE = 12,     // ts respond to mirror with collected galleries
    CMD_ID_GALLERY_REQUEST = 13,      // mirror request tc to collect galleries
    CMD_ID_GALLERY_REGISTER = 14,     // mirror request tc to register galleries
    CMD_ID_INVALID,              // 无效状态
};

// 一堂课的状态
enum CLASS_STATE {
    CLASS_STATE_NOT_START = 0,      // 未开始上课状态
    CLASS_STATE_LISTEN = 1,         // 听课区间
    CLASS_STATE_BREAK_TIMES = 2,    // 课间休息
    CLASS_STATE_EXERCISE = 3,       // 做题
    CLASS_STATE_INTERACTION = 4,    // 互动
    CLASS_STATE_END = 5,            // 结束上课状态

    BRING_BACK_STATE_LIST = 10,     // 收心
    CLASS_STATE_MAX,
};


// 数据类型
enum DATA_TYPE {
    DATA_TYPE_CLASS = 1,        // 班级听课数据
    DATA_TYPE_BAD_STU = 2,      // 学生不听课数据
    DATA_TYPE_PRAISE_LIST = 3,  // 表扬列表
    DATA_TYPE_PEAK_PIC = 4,     // 学生图片列表
    DATA_TYPE_STUS_IMG = 5,     // 全部学生头像
};

// 底层识别状态码
enum STU_STATUS {
    STU_STATUS_LISTEN = 1001,          // 听课
    STU_STATUS_BOOK = 1002,            // 看书
    STU_STATUS_WRITE = 1003,           // 写字
    STU_STATUS_HAND = 1004,            // 举手
    STU_STATUS_LAUGH = 1005,           // 微笑
    STU_STATUS_OPEN_MOUSE = 1006,      // 张嘴
    STU_STATUS_OTHER_POSITIVE = 1007,  // 正向的
    STU_STATUS_OTHER_NEGATIVE = 1008,  // 负向的

    STU_STATUS_GLANCED_OVER = 2001,    // 扭头
    STU_STATUS_PLAY = 2002,            // 玩东西
    STU_STATUS_BOW = 2003,             // 低头
    STU_STATUS_EXPRESSIONLESS = 2004,  // 正脸无表情
    STU_STATUS_HEAD_OTHER = 2005,      // 头部其他
};

enum STU_EMOTION {
    STU_EMOTION_NEGATIVE = 1001,  // 负向的
    STU_EMOTION_HAPPY = 1002,  // 微笑
    STU_EMOTION_NEUTRAL = 1003,  // 正常
};

// 文件上传的服务器类型
enum UPLOAD_TYPE {
    UPLOAD_TYPE_START = 0,
    UPLOAD_TYPE_OSS_ALI,
    UPLOAD_TYPE_OSS_QINIU,
    UPLOAD_TYPE_FTP,
    UPLOAD_TYPE_HTTP,
    UPLOAD_TYPE_OSS_TAL, //zgc 20180423 add
    UPLOAD_TYPE_END,
};

// 上传的数据类型
enum FILE_TYPE {
    FILE_TYPE_BUFFER = 0,
    FILE_TYPE_LOCAL_IMAGE,
    FILE_TYPE_LOCAL_VIDEO,
    FILE_TYPE_STREAM,
};

enum MSG_TYPE {
    MSG_TYPE_ClientData = 0,
    MSG_TYPE_CommonData,
    MSG_TYPE_StudentHeadImg,
    MSG_TYPE_ClassData,
    MSG_TYPE_Video,
    MSG_TYPE_SmileImage,
    MSG_TYPE_HandupImage,
    MSG_TYPE_ClassStatusChange
};

enum MSG_CLIENT_TYPE {
    MSG_CLIENT_TYPE_IDENT = 1,  // 1 识别端(identify)
    MSG_CLIENT_TYPE_STU,        // 2 学生端大屏幕(student)
    MSG_CLIENT_TYPE_MASTER,     // 3 主讲pad(master)
    MSG_CLIENT_TYPE_ASSIST,     // 4 辅讲pad(assist)
};

struct StudentData {
    unsigned int id;
    unsigned int head_status;
    unsigned int head_conf;
    unsigned int body_status;
    unsigned int body_conf;
    unsigned int x;
    unsigned int y;
    unsigned int width;
    unsigned int height;
    unsigned int head_x;
    unsigned int head_y;
    unsigned int head_width;
    unsigned int head_height;
    std::string img;
    unsigned int frm_num;

    StudentData() {
        id = 0;
        head_status = 0;
        head_conf = 0;
        body_status = 0;
        body_conf = 0;
        x = 0;
        y = 0;
        width = 0;
        height = 0;
        head_x = 0;
        head_y = 0;
        head_width = 0;
        head_height = 0;
        img = "";
        frm_num = 0;
    }
};

//客户端建立连接 发送的数据。
struct Stclient {
    int channel_id;
    std::string client_id;
    std::string class_id;
    std::string live_id;
    int    cuc_num;
    int    client_type;
    std::string token;
    Stclient() {
        live_id = "";
        class_id = "";
        cuc_num = 0;
        client_type = 0;
        token = "";
        client_id = "";
        channel_id = 0;
    }
};

//1 识别端发送一般数据
struct CommonData {
    int channel_id;
    std::string client_id;
    std::string class_id;
    std::string op_type;  // 消息类型
    std::string startime; // 开始时间
    std::string endtime;  //	结束时间
    std::string video_start_timestamp; //视频开始时间戳
    std::string uuid;
    int fps;                     //每秒多少帧
    int d_freq;
};

//2 识别端发送头像数据
//单个学生头像数据
struct Studentdata {
    int stu_id;         //学生ID
    std::string std_head_img;  //学生头像
};

struct HeadImgData {
    int channel_id;
    std::string client_id;
    std::string class_id;
    std::string  op_type; //
    std::list <Studentdata> st_studentdata; //
};

//3 识别端发送的原始数据
struct  Siglestudent {
    int headstu; //头部状态
    int bodystu; //身体状态
};

struct Stuconfidence {
    int headconfidence;
    int bodyconfidence;
};

struct Students {
    int map_stu_id; //学生Id
    Siglestudent stu_status; //学生数据
    Stuconfidence stu_confidence; //置信度
};

struct OrignalStudentdata {
    int channel_id;
    std::string client_id;
    std::string class_id;
    std::string op_type; //识别标识
    int frm_num;  //帧数
    std::string time_stamp;//时间戳
    int class_status;	   //班级状态
    std::list <Students> studentdata; //原始数据list
};

//识别端发送小视频数据
struct StudentVideo {
    int conf;
    int status;
    std::string stu_video_img;  //学生小视频图像
    std::string stu_video;  // 学生小视频链接
};

//识别端发送小视频数据
struct StudentVideos {
    int stu_id;         //学生ID
    std::list<StudentVideo> video_list;
};

// 识别端发送上课状态数据 zgc 20180614 add
struct ClassStatusChange {
    int channel_id;
    std::string client_id;
    std::string class_id;
    int class_type;		// 0:课间休息 1:课间休息结束，开始上课
    std::string timestamp; //视频开始时间戳
    ClassStatusChange() {
        class_type = -1;
        timestamp = "";
    }
};
struct StudentVideoList {
    int channel_id;
    std::string client_id;
    std::string class_id;
    std::string  op_type; //
    std::list <StudentVideos> studentdata; //
};

// 针对有课程信息的toG toB项目
struct Course {
    int id;
    std::string class_id;
    std::string teacher_id;
    std::string course_name;
    int start_timestamp;
    int end_timestamp;

    Course() {
        id = 0;
        class_id = "";
        teacher_id = "";
        course_name = "";
        start_timestamp = 0;
        end_timestamp = 0;
    }
};
//维度瞬间 zgc 20180530 add
struct MomentInfo {
    std::vector<std::pair<int, std::string>>
                                          vconf_video; //<confidence_num, file_video_name>
    int last_record_video_time;
    int min_conf_num;                                     //min confidence num

    MomentInfo() {
        clear();
    }
    void clear() {
        vconf_video.clear();
        last_record_video_time = 0;
        min_conf_num = 0;
    }
};
#endif
