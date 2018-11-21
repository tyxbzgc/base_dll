// Copyright (c) 2017 The TAL Authors.
// All rights reserved.


#ifndef BASE_COMMON_H_
#define BASE_COMMON_H_

#include <list>
#include <vector>
#include <string.h>

/*¼����Ƶ���*/
// �ͻ��˵�������
#define VIDEO_RECORD_END 0
#define VIDEO_RECORD_START 1

// ���������ͻ���
#define VIDEO_RECORD_SUCCESS 0
#define VIDEO_RECORD_FAIL 1

// config �ļ����
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

// ��¼��ѧ����һ֡����״̬��down�½� up����
#define STU_SCORE_STATUS_DOWN = "down"
#define STU_SCORE_STATUS_UP = "up"

// �ϴ��ļ�����
#define UPLOAD_FILE_TYPE_VIDEO 0
#define UPLOAD_FILE_TYPE_IMAGE 1

// ¼�Ʊ�����Ƶ
#define LOCAL_VIDEO_STOP 0
#define LOCAL_VIDEO_START 1
#define LOCAL_VIDEO_PAUSE 2

#define DATA_REPORT_HOST "101.201.118.73"
#define DATA_REPORT_PORT 8080
#define DATA_REPORT_URL "/api/v1/data-report"
// URL���
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
// uuidע���
#define UUID_REG_KEY L"SOFTWARE\\AILab\\Cryptography"
#define UUID_REG_SUBKEY L"uuid"
#define PRODUCT_REG_KEY L"SOFTWARE\\TAL\\Mirror"
#define PRODUCT_REG_VERSION L"Version"

// ������Ϣ
enum CHANNEL_INFO {
    CHANNEL_START = 0,
    CHANNEL_DOUBLE_TEACHER,  // ˫ʦ
    CHANNEL_TOG,  // ����У
    CHANNEL_DAHAI,
    CHANNEL_WEIDU,
    CHANNEL_END
};

// �ϴ�����������
enum CLIENT_TYPE {
    CLIENT_TYPE_START = 0,
    CLIENT_TYPE_CLIENT,
    CLIENT_TYPE_SERVER,
    CLIENT_TYPE_END,
};

// ��־�ȼ�
enum LOG_LEVEL {
    LOG_DEBUG = 0,
    LOG_NOTIFICATION,
    LOG_WARING,
    LOG_ERROR,
    LOG_FATAL,
};

// ��FTЭ���ֶ�
enum CMD_ID {
    CMD_ID_START = 0,
    CMD_ID_STU_DATA = 1,              // ѧ���༶��Ϣ
    CMD_ID_CLASS_STATE = 2,           // �༶״̬����
    CMD_ID_CONFIG_DATA = 3,           // ��ʼ����������
    CMD_ID_VIDEO_RECORD = 4,          // ������Ƶ¼����Ϣ
    CMD_ID_CLASS_INFO = 5,            // �༶��Ϣ
    CMD_ID_STU_MAP = 6,               // ѧ��IDӳ���
    CMD_ID_UPLOAD_VIDEO = 7,          // �Ƿ��ϴ�С��Ƶ
    CMD_ID_NOTIFY_STATE = 8,          // ֪ͨ����״̬
    CMD_ID_RECORD_VIDEO_CONTROL = 9,  // ������Ƶ¼��
    CMD_ID_FRAME_DATA,                // ÿ֡����
    CMD_ID_CAMERA_STATUS,             // ����ͷ״̬
    CMD_ID_GALLERY_RESPONSE = 12,     // ts respond to mirror with collected galleries
    CMD_ID_GALLERY_REQUEST = 13,      // mirror request tc to collect galleries
    CMD_ID_GALLERY_REGISTER = 14,     // mirror request tc to register galleries
    CMD_ID_INVALID,              // ��Ч״̬
};

// һ�ÿε�״̬
enum CLASS_STATE {
    CLASS_STATE_NOT_START = 0,      // δ��ʼ�Ͽ�״̬
    CLASS_STATE_LISTEN = 1,         // ��������
    CLASS_STATE_BREAK_TIMES = 2,    // �μ���Ϣ
    CLASS_STATE_EXERCISE = 3,       // ����
    CLASS_STATE_INTERACTION = 4,    // ����
    CLASS_STATE_END = 5,            // �����Ͽ�״̬

    BRING_BACK_STATE_LIST = 10,     // ����
    CLASS_STATE_MAX,
};


// ��������
enum DATA_TYPE {
    DATA_TYPE_CLASS = 1,        // �༶��������
    DATA_TYPE_BAD_STU = 2,      // ѧ������������
    DATA_TYPE_PRAISE_LIST = 3,  // �����б�
    DATA_TYPE_PEAK_PIC = 4,     // ѧ��ͼƬ�б�
    DATA_TYPE_STUS_IMG = 5,     // ȫ��ѧ��ͷ��
};

// �ײ�ʶ��״̬��
enum STU_STATUS {
    STU_STATUS_LISTEN = 1001,          // ����
    STU_STATUS_BOOK = 1002,            // ����
    STU_STATUS_WRITE = 1003,           // д��
    STU_STATUS_HAND = 1004,            // ����
    STU_STATUS_LAUGH = 1005,           // ΢Ц
    STU_STATUS_OPEN_MOUSE = 1006,      // ����
    STU_STATUS_OTHER_POSITIVE = 1007,  // �����
    STU_STATUS_OTHER_NEGATIVE = 1008,  // �����

    STU_STATUS_GLANCED_OVER = 2001,    // Ťͷ
    STU_STATUS_PLAY = 2002,            // �涫��
    STU_STATUS_BOW = 2003,             // ��ͷ
    STU_STATUS_EXPRESSIONLESS = 2004,  // �����ޱ���
    STU_STATUS_HEAD_OTHER = 2005,      // ͷ������
};

enum STU_EMOTION {
    STU_EMOTION_NEGATIVE = 1001,  // �����
    STU_EMOTION_HAPPY = 1002,  // ΢Ц
    STU_EMOTION_NEUTRAL = 1003,  // ����
};

// �ļ��ϴ��ķ���������
enum UPLOAD_TYPE {
    UPLOAD_TYPE_START = 0,
    UPLOAD_TYPE_OSS_ALI,
    UPLOAD_TYPE_OSS_QINIU,
    UPLOAD_TYPE_FTP,
    UPLOAD_TYPE_HTTP,
    UPLOAD_TYPE_OSS_TAL, //zgc 20180423 add
    UPLOAD_TYPE_END,
};

// �ϴ�����������
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
    MSG_CLIENT_TYPE_IDENT = 1,  // 1 ʶ���(identify)
    MSG_CLIENT_TYPE_STU,        // 2 ѧ���˴���Ļ(student)
    MSG_CLIENT_TYPE_MASTER,     // 3 ����pad(master)
    MSG_CLIENT_TYPE_ASSIST,     // 4 ����pad(assist)
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

//�ͻ��˽������� ���͵����ݡ�
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

//1 ʶ��˷���һ������
struct CommonData {
    int channel_id;
    std::string client_id;
    std::string class_id;
    std::string op_type;  // ��Ϣ����
    std::string startime; // ��ʼʱ��
    std::string endtime;  //	����ʱ��
    std::string video_start_timestamp; //��Ƶ��ʼʱ���
    std::string uuid;
    int fps;                     //ÿ�����֡
    int d_freq;
};

//2 ʶ��˷���ͷ������
//����ѧ��ͷ������
struct Studentdata {
    int stu_id;         //ѧ��ID
    std::string std_head_img;  //ѧ��ͷ��
};

struct HeadImgData {
    int channel_id;
    std::string client_id;
    std::string class_id;
    std::string  op_type; //
    std::list <Studentdata> st_studentdata; //
};

//3 ʶ��˷��͵�ԭʼ����
struct  Siglestudent {
    int headstu; //ͷ��״̬
    int bodystu; //����״̬
};

struct Stuconfidence {
    int headconfidence;
    int bodyconfidence;
};

struct Students {
    int map_stu_id; //ѧ��Id
    Siglestudent stu_status; //ѧ������
    Stuconfidence stu_confidence; //���Ŷ�
};

struct OrignalStudentdata {
    int channel_id;
    std::string client_id;
    std::string class_id;
    std::string op_type; //ʶ���ʶ
    int frm_num;  //֡��
    std::string time_stamp;//ʱ���
    int class_status;	   //�༶״̬
    std::list <Students> studentdata; //ԭʼ����list
};

//ʶ��˷���С��Ƶ����
struct StudentVideo {
    int conf;
    int status;
    std::string stu_video_img;  //ѧ��С��Ƶͼ��
    std::string stu_video;  // ѧ��С��Ƶ����
};

//ʶ��˷���С��Ƶ����
struct StudentVideos {
    int stu_id;         //ѧ��ID
    std::list<StudentVideo> video_list;
};

// ʶ��˷����Ͽ�״̬���� zgc 20180614 add
struct ClassStatusChange {
    int channel_id;
    std::string client_id;
    std::string class_id;
    int class_type;		// 0:�μ���Ϣ 1:�μ���Ϣ��������ʼ�Ͽ�
    std::string timestamp; //��Ƶ��ʼʱ���
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

// ����пγ���Ϣ��toG toB��Ŀ
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
//ά��˲�� zgc 20180530 add
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
