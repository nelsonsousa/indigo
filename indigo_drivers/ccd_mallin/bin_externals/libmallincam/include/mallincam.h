#ifndef __mallincam_h__
#define __mallincam_h__

/* Version: 49.18608.20210313 */
/*
   Platform & Architecture:
       (1) Win32:
              (a) x86: XP SP3 or above; CPU supports SSE2 instruction set or above
              (b) x64: Win7 or above
              (c) arm: Win10 or above
              (d) arm64: Win10 or above
       (2) WinRT: x86, x64, arm, arm64; Win10 or above
       (3) macOS: universal (x64 + x86); macOS 10.10 or above
       (4) Linux: kernel 2.6.27 or above; GLIBC 2.17 or above
              (a) x86: CPU supports SSE3 instruction set or above; built by gcc 5.4.0
              (b) x64: built by gcc 5.4.0
              (c) armel: built by toolchain arm-linux-gnueabi (version 4.9.2)
              (d) armhf: built by toolchain arm-linux-gnueabihf (version 4.9.2)
              (e) arm64: built by toolchain aarch64-linux-gnu (version 4.9.2)
       (5) Android: arm, arm64, x86, x64; built by android-ndk-r18b; __ANDROID_API__ = 23
*/
/*
    doc:
       (1) en.html, English
       (2) hans.html, Simplified Chinese
*/

#ifdef _WIN32
#ifndef _INC_WINDOWS
#include <windows.h>
#endif
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if defined(__cplusplus) && (__cplusplus >= 201402L)
#define MALLINCAM_DEPRECATED  [[deprecated]]
#elif defined(_MSC_VER)
#define MALLINCAM_DEPRECATED  __declspec(deprecated)
#elif defined(__GNUC__) || defined(__clang__)
#define MALLINCAM_DEPRECATED  __attribute__((deprecated))
#else
#define MALLINCAM_DEPRECATED
#endif

#ifdef _WIN32 /* Windows */

#pragma pack(push, 8)
#ifdef MALLINCAM_EXPORTS
#define MALLINCAM_API(x)    __declspec(dllexport)   x   __stdcall  /* in Windows, we use __stdcall calling convention, see https://docs.microsoft.com/en-us/cpp/cpp/stdcall */
#elif !defined(MALLINCAM_NOIMPORTS)
#define MALLINCAM_API(x)    __declspec(dllimport)   x   __stdcall
#else
#define MALLINCAM_API(x)    x   __stdcall
#endif

#else   /* Linux or macOS */

#define MALLINCAM_API(x)    x
#if (!defined(HRESULT)) && (!defined(__COREFOUNDATION_CFPLUGINCOM__)) /* CFPlugInCOM.h */
#define HRESULT int
#endif
#ifndef SUCCEEDED
#define SUCCEEDED(hr)   (((HRESULT)(hr)) >= 0)
#endif
#ifndef FAILED
#define FAILED(hr)      (((HRESULT)(hr)) < 0)
#endif

#ifndef __stdcall
#define __stdcall
#endif

#ifndef __BITMAPINFOHEADER_DEFINED__
#define __BITMAPINFOHEADER_DEFINED__
typedef struct {
    unsigned        biSize;
    int             biWidth;
    int             biHeight;
    unsigned short  biPlanes;
    unsigned short  biBitCount;
    unsigned        biCompression;
    unsigned        biSizeImage;
    int             biXPelsPerMeter;
    int             biYPelsPerMeter;
    unsigned        biClrUsed;
    unsigned        biClrImportant;
} BITMAPINFOHEADER;
#endif

#ifndef __RECT_DEFINED__
#define __RECT_DEFINED__
typedef struct {
    int left;
    int top;
    int right;
    int bottom;
} RECT, *PRECT;
#endif

#endif

#ifndef TDIBWIDTHBYTES
#define TDIBWIDTHBYTES(bits)  ((unsigned)(((bits) + 31) & (~31)) / 8)
#endif

/*****************************************************************************************************************/
/* HRESULT                                                                                                       */
/*    |----------------|------------------------------------------------------------------------|------------|   */
/*    | S_OK           |   Operation successful                                                 | 0x00000000 |   */
/*    | S_FALSE        |   Operation successful                                                 | 0x00000001 |   */
/*    | E_FAIL         |   Unspecified failure                                                  | 0x80004005 |   */
/*    | E_ACCESSDENIED |   General access denied error                                          | 0x80070005 |   */
/*    | E_INVALIDARG   |   One or more arguments are not valid                                  | 0x80070057 |   */
/*    | E_NOTIMPL      |   Not supported or not implemented                                     | 0x80004001 |   */
/*    | E_NOINTERFACE  |   Interface not supported                                              | 0x80004002 |   */
/*    | E_POINTER      |   Pointer that is not valid                                            | 0x80004003 |   */
/*    | E_UNEXPECTED   |   Unexpected failure                                                   | 0x8000FFFF |   */
/*    | E_OUTOFMEMORY  |   Out of memory                                                        | 0x8007000E |   */
/*    | E_WRONG_THREAD |   call function in the wrong thread                                    | 0x8001010E |   */
/*    | E_GEN_FAILURE  |   device not functioning                                               | 0x8007001F |   */
/*    | E_PENDING      |   The data necessary to complete this operation is not yet available   | 0x8000000A |   */
/*    |----------------|------------------------------------------------------------------------|------------|   */
/*****************************************************************************************************************/
/*                                                                              */
/* Please note that the return value >= 0 means success                         */
/* (especially S_FALSE is also successful, indicating that the internal value and the value set by the user is equivalent, which means "no operation"). */
/* Therefore, the SUCCEEDEDand FAILED macros should generally be used to determine whether the return value is successful or failed. */
/* (Unless there are special needs, do not use "==S_OK" or "==0" to judge the return value) */
/*                                                                              */
/* #define SUCCEEDED(hr)   (((HRESULT)(hr)) >= 0)                               */
/* #define FAILED(hr)      (((HRESULT)(hr)) < 0)                                */
/*                                                                              */
/********************************************************************************/

/* handle */
typedef struct ToupcamT { int unused; } *HToupcam, *HToupCam;

#define MALLINCAM_MAX                      16
                                         
#define MALLINCAM_FLAG_CMOS                0x00000001  /* cmos sensor */
#define MALLINCAM_FLAG_CCD_PROGRESSIVE     0x00000002  /* progressive ccd sensor */
#define MALLINCAM_FLAG_CCD_INTERLACED      0x00000004  /* interlaced ccd sensor */
#define MALLINCAM_FLAG_ROI_HARDWARE        0x00000008  /* support hardware ROI */
#define MALLINCAM_FLAG_MONO                0x00000010  /* monochromatic */
#define MALLINCAM_FLAG_BINSKIP_SUPPORTED   0x00000020  /* support bin/skip mode, see Toupcam_put_Mode and Toupcam_get_Mode */
#define MALLINCAM_FLAG_USB30               0x00000040  /* usb3.0 */
#define MALLINCAM_FLAG_TEC                 0x00000080  /* Thermoelectric Cooler */
#define MALLINCAM_FLAG_USB30_OVER_USB20    0x00000100  /* usb3.0 camera connected to usb2.0 port */
#define MALLINCAM_FLAG_ST4                 0x00000200  /* ST4 port */
#define MALLINCAM_FLAG_GETTEMPERATURE      0x00000400  /* support to get the temperature of the sensor */
#define MALLINCAM_FLAG_RAW10               0x00001000  /* pixel format, RAW 10bits */
#define MALLINCAM_FLAG_RAW12               0x00002000  /* pixel format, RAW 12bits */
#define MALLINCAM_FLAG_RAW14               0x00004000  /* pixel format, RAW 14bits */
#define MALLINCAM_FLAG_RAW16               0x00008000  /* pixel format, RAW 16bits */
#define MALLINCAM_FLAG_FAN                 0x00010000  /* cooling fan */
#define MALLINCAM_FLAG_TEC_ONOFF           0x00020000  /* Thermoelectric Cooler can be turn on or off, support to set the target temperature of TEC */
#define MALLINCAM_FLAG_ISP                 0x00040000  /* ISP (Image Signal Processing) chip */
#define MALLINCAM_FLAG_TRIGGER_SOFTWARE    0x00080000  /* support software trigger */
#define MALLINCAM_FLAG_TRIGGER_EXTERNAL    0x00100000  /* support external trigger */
#define MALLINCAM_FLAG_TRIGGER_SINGLE      0x00200000  /* only support trigger single: one trigger, one image */
#define MALLINCAM_FLAG_BLACKLEVEL          0x00400000  /* support set and get the black level */
#define MALLINCAM_FLAG_AUTO_FOCUS          0x00800000  /* support auto focus */
#define MALLINCAM_FLAG_BUFFER              0x01000000  /* frame buffer */
#define MALLINCAM_FLAG_DDR                 0x02000000  /* use very large capacity DDR (Double Data Rate SDRAM) for frame buffer */
#define MALLINCAM_FLAG_CG                  0x04000000  /* Conversion Gain: HCG, LCG */
#define MALLINCAM_FLAG_YUV411              0x08000000  /* pixel format, yuv411 */
#define MALLINCAM_FLAG_VUYY                0x10000000  /* pixel format, yuv422, VUYY */
#define MALLINCAM_FLAG_YUV444              0x20000000  /* pixel format, yuv444 */
#define MALLINCAM_FLAG_RGB888              0x40000000  /* pixel format, RGB888 */
#define MALLINCAM_FLAG_RAW8                0x80000000  /* pixel format, RAW 8 bits */
#define MALLINCAM_FLAG_GMCY8               0x0000000100000000  /* pixel format, GMCY, 8bits */
#define MALLINCAM_FLAG_GMCY12              0x0000000200000000  /* pixel format, GMCY, 12bits */
#define MALLINCAM_FLAG_UYVY                0x0000000400000000  /* pixel format, yuv422, UYVY */
#define MALLINCAM_FLAG_CGHDR               0x0000000800000000  /* Conversion Gain: HCG, LCG, HDR */
#define MALLINCAM_FLAG_GLOBALSHUTTER       0x0000001000000000  /* global shutter */
#define MALLINCAM_FLAG_FOCUSMOTOR          0x0000002000000000  /* support focus motor */
#define MALLINCAM_FLAG_PRECISE_FRAMERATE   0x0000004000000000  /* support precise framerate & bandwidth, see MALLINCAM_OPTION_PRECISE_FRAMERATE & MALLINCAM_OPTION_BANDWIDTH */
#define MALLINCAM_FLAG_HEAT                0x0000008000000000  /* support heat to prevent fogging up */
#define MALLINCAM_FLAG_LOW_NOISE           0x0000010000000000  /* support low noise mode (Higher signal noise ratio, lower frame rate) */
#define MALLINCAM_FLAG_LEVELRANGE_HARDWARE 0x0000020000000000  /* hardware level range, put(get)_LevelRangeV2 */
#define MALLINCAM_FLAG_EVENT_HARDWARE      0x0000040000000000  /* hardware event, such as exposure start & stop */

#define MALLINCAM_TEMP_DEF                 6503    /* temp, default */
#define MALLINCAM_TEMP_MIN                 2000    /* temp, minimum */
#define MALLINCAM_TEMP_MAX                 15000   /* temp, maximum */
#define MALLINCAM_TINT_DEF                 1000    /* tint */
#define MALLINCAM_TINT_MIN                 200     /* tint */
#define MALLINCAM_TINT_MAX                 2500    /* tint */
#define MALLINCAM_HUE_DEF                  0       /* hue */
#define MALLINCAM_HUE_MIN                  (-180)  /* hue */
#define MALLINCAM_HUE_MAX                  180     /* hue */
#define MALLINCAM_SATURATION_DEF           128     /* saturation */
#define MALLINCAM_SATURATION_MIN           0       /* saturation */
#define MALLINCAM_SATURATION_MAX           255     /* saturation */
#define MALLINCAM_BRIGHTNESS_DEF           0       /* brightness */
#define MALLINCAM_BRIGHTNESS_MIN           (-64)   /* brightness */
#define MALLINCAM_BRIGHTNESS_MAX           64      /* brightness */
#define MALLINCAM_CONTRAST_DEF             0       /* contrast */
#define MALLINCAM_CONTRAST_MIN             (-100)  /* contrast */
#define MALLINCAM_CONTRAST_MAX             100     /* contrast */
#define MALLINCAM_GAMMA_DEF                100     /* gamma */
#define MALLINCAM_GAMMA_MIN                20      /* gamma */
#define MALLINCAM_GAMMA_MAX                180     /* gamma */
#define MALLINCAM_AETARGET_DEF             120     /* target of auto exposure */
#define MALLINCAM_AETARGET_MIN             16      /* target of auto exposure */
#define MALLINCAM_AETARGET_MAX             220     /* target of auto exposure */
#define MALLINCAM_WBGAIN_DEF               0       /* white balance gain */
#define MALLINCAM_WBGAIN_MIN               (-127)  /* white balance gain */
#define MALLINCAM_WBGAIN_MAX               127     /* white balance gain */
#define MALLINCAM_BLACKLEVEL_MIN           0       /* minimum black level */
#define MALLINCAM_BLACKLEVEL8_MAX          31              /* maximum black level for bit depth = 8 */
#define MALLINCAM_BLACKLEVEL10_MAX         (31 * 4)        /* maximum black level for bit depth = 10 */
#define MALLINCAM_BLACKLEVEL12_MAX         (31 * 16)       /* maximum black level for bit depth = 12 */
#define MALLINCAM_BLACKLEVEL14_MAX         (31 * 64)       /* maximum black level for bit depth = 14 */
#define MALLINCAM_BLACKLEVEL16_MAX         (31 * 256)      /* maximum black level for bit depth = 16 */
#define MALLINCAM_SHARPENING_STRENGTH_DEF  0       /* sharpening strength */
#define MALLINCAM_SHARPENING_STRENGTH_MIN  0       /* sharpening strength */
#define MALLINCAM_SHARPENING_STRENGTH_MAX  500     /* sharpening strength */
#define MALLINCAM_SHARPENING_RADIUS_DEF    2       /* sharpening radius */
#define MALLINCAM_SHARPENING_RADIUS_MIN    1       /* sharpening radius */
#define MALLINCAM_SHARPENING_RADIUS_MAX    10      /* sharpening radius */
#define MALLINCAM_SHARPENING_THRESHOLD_DEF 0       /* sharpening threshold */
#define MALLINCAM_SHARPENING_THRESHOLD_MIN 0       /* sharpening threshold */
#define MALLINCAM_SHARPENING_THRESHOLD_MAX 255     /* sharpening threshold */
#define MALLINCAM_AUTOEXPO_THRESHOLD_DEF   5       /* auto exposure threshold */
#define MALLINCAM_AUTOEXPO_THRESHOLD_MIN   2       /* auto exposure threshold */
#define MALLINCAM_AUTOEXPO_THRESHOLD_MAX   15      /* auto exposure threshold */
#define MALLINCAM_BANDWIDTH_DEF            90      /* bandwidth */
#define MALLINCAM_BANDWIDTH_MIN            1       /* bandwidth */
#define MALLINCAM_BANDWIDTH_MAX            100     /* bandwidth */
#define MALLINCAM_DENOISE_DEF              0       /* denoise */
#define MALLINCAM_DENOISE_MIN              0       /* denoise */
#define MALLINCAM_DENOISE_MAX              100     /* denoise */

typedef struct{
    unsigned    width;
    unsigned    height;
}ToupcamResolution;

/* In Windows platform, we always use UNICODE wchar_t */
/* In Linux or macOS, we use char */

typedef struct {
#ifdef _WIN32
    const wchar_t*      name;        /* model name, in Windows, we use unicode */
#else
    const char*         name;        /* model name */
#endif
    unsigned long long  flag;        /* MALLINCAM_FLAG_xxx, 64 bits */
    unsigned            maxspeed;    /* number of speed level, same as Toupcam_get_MaxSpeed(), the speed range = [0, maxspeed], closed interval */
    unsigned            preview;     /* number of preview resolution, same as Toupcam_get_ResolutionNumber() */
    unsigned            still;       /* number of still resolution, same as Toupcam_get_StillResolutionNumber() */
    unsigned            maxfanspeed; /* maximum fan speed */
    unsigned            ioctrol;     /* number of input/output control */
    float               xpixsz;      /* physical pixel size */
    float               ypixsz;      /* physical pixel size */
    ToupcamResolution   res[MALLINCAM_MAX];
}ToupcamModelV2; /* camera model v2 */

typedef struct {
#ifdef _WIN32
    wchar_t               displayname[64];    /* display name */
    wchar_t               id[64];             /* unique and opaque id of a connected camera, for Toupcam_Open */
#else
    char                  displayname[64];    /* display name */
    char                  id[64];             /* unique and opaque id of a connected camera, for Toupcam_Open */
#endif
    const ToupcamModelV2* model;
}ToupcamDeviceV2; /* camera instance for enumerating */

/*
    get the version of this dll/so/dylib, which is: 49.18608.20210313
*/
#ifdef _WIN32
MALLINCAM_API(const wchar_t*)   Toupcam_Version();
#else
MALLINCAM_API(const char*)      Toupcam_Version();
#endif

/*
    enumerate the cameras connected to the computer, return the number of enumerated.

    ToupcamDeviceV2 arr[MALLINCAM_MAX];
    unsigned cnt = Toupcam_EnumV2(arr);
    for (unsigned i = 0; i < cnt; ++i)
        ...

    if pti == NULL, then, only the number is returned.
    Toupcam_Enum is obsolete.
*/
MALLINCAM_API(unsigned) Toupcam_EnumV2(ToupcamDeviceV2 pti[MALLINCAM_MAX]);

/* use the id of ToupcamDeviceV2, which is enumerated by Toupcam_EnumV2.
    if id is NULL, Toupcam_Open will open the first camera.
*/
#ifdef _WIN32
MALLINCAM_API(HToupcam) Toupcam_Open(const wchar_t* id);
#else
MALLINCAM_API(HToupcam) Toupcam_Open(const char* id);
#endif

/*
    the same with Toupcam_Open, but use the index as the parameter. such as:
    index == 0, open the first camera,
    index == 1, open the second camera,
    etc
*/
MALLINCAM_API(HToupcam) Toupcam_OpenByIndex(unsigned index);

MALLINCAM_API(void)     Toupcam_Close(HToupcam h);  /* close the handle */

#define MALLINCAM_EVENT_EXPOSURE          0x0001    /* exposure time or gain changed */
#define MALLINCAM_EVENT_TEMPTINT          0x0002    /* white balance changed, Temp/Tint mode */
#define MALLINCAM_EVENT_IMAGE             0x0004    /* live image arrived, use Toupcam_PullImage to get this image */
#define MALLINCAM_EVENT_STILLIMAGE        0x0005    /* snap (still) frame arrived, use Toupcam_PullStillImage to get this frame */
#define MALLINCAM_EVENT_WBGAIN            0x0006    /* white balance changed, RGB Gain mode */
#define MALLINCAM_EVENT_TRIGGERFAIL       0x0007    /* trigger failed */
#define MALLINCAM_EVENT_BLACK             0x0008    /* black balance changed */
#define MALLINCAM_EVENT_FFC               0x0009    /* flat field correction status changed */
#define MALLINCAM_EVENT_DFC               0x000a    /* dark field correction status changed */
#define MALLINCAM_EVENT_ROI               0x000b    /* roi changed */
#define MALLINCAM_EVENT_LEVELRANGE        0x000c    /* level range changed */
#define MALLINCAM_EVENT_ERROR             0x0080    /* generic error */
#define MALLINCAM_EVENT_DISCONNECTED      0x0081    /* camera disconnected */
#define MALLINCAM_EVENT_NOFRAMETIMEOUT    0x0082    /* no frame timeout error */
#define MALLINCAM_EVENT_AFFEEDBACK        0x0083    /* auto focus feedback information */
#define MALLINCAM_EVENT_AFPOSITION        0x0084    /* auto focus sensor board positon */
#define MALLINCAM_EVENT_NOPACKETTIMEOUT   0x0085    /* no packet timeout */
#define MALLINCAM_EVENT_EXPO_START        0x4000    /* exposure start */
#define MALLINCAM_EVENT_EXPO_STOP         0x4001    /* exposure stop */
#define MALLINCAM_EVENT_TRIGGER_ALLOW     0x4002    /* next trigger allow */
#define MALLINCAM_EVENT_FACTORY           0x8001    /* restore factory settings */

#ifdef _WIN32
MALLINCAM_API(HRESULT)  Toupcam_StartPullModeWithWndMsg(HToupcam h, HWND hWnd, UINT nMsg);
#endif

/* Do NOT call Toupcam_Close, Toupcam_Stop in this callback context, it deadlocks. */
typedef void (__stdcall* PMALLINCAM_EVENT_CALLBACK)(unsigned nEvent, void* pCallbackCtx);
MALLINCAM_API(HRESULT)  Toupcam_StartPullModeWithCallback(HToupcam h, PMALLINCAM_EVENT_CALLBACK pEventCallback, void* pCallbackContext);

#define MALLINCAM_FRAMEINFO_FLAG_SEQ          0x01 /* sequence number */
#define MALLINCAM_FRAMEINFO_FLAG_TIMESTAMP    0x02 /* timestamp */

typedef struct {
    unsigned            width;
    unsigned            height;
    unsigned            flag;       /* MALLINCAM_FRAMEINFO_FLAG_xxxx */
    unsigned            seq;        /* sequence number */
    unsigned long long  timestamp;  /* microsecond */
}ToupcamFrameInfoV2;

/*
    bits: 24 (RGB24), 32 (RGB32), 48 (RGB48), 8 (Gray) or 16 (Gray). In RAW mode, this parameter is ignored.
    pnWidth, pnHeight: OUT parameter
    rowPitch: The distance from one row to the next row. rowPitch = 0 means using the default row pitch. rowPitch = -1 means zero padding
    
    -------------------------------------------------------------------------------------------------------------
    | format                                            | 0 means default row pitch     | -1 means zero padding |
    |---------------------------------------------------|-------------------------------|-----------------------|
    | RGB       | RGB24                                 | TDIBWIDTHBYTES(24 * Width)    | Width * 3             |
    |           | RGB32                                 | Width * 4                     | Width * 4             |
    |           | RGB48                                 | TDIBWIDTHBYTES(48 * Width)    | Width * 6             |
    |           | GREY8                                 | TDIBWIDTHBYTES(8 * Width)     | Width                 |
    |           | GREY16                                | TDIBWIDTHBYTES(16 * Width)    | Width * 2             |
    |-----------|---------------------------------------|-------------------------------|-----------------------|
    | RAW       | 8bits Mode                            | Width                         | Width                 |
    |           | 10bits, 12bits, 14bits, 16bits Mode   | Width * 2                     | Width * 2             |
    |-----------|---------------------------------------|-------------------------------|-----------------------|
*/
MALLINCAM_API(HRESULT)  Toupcam_PullImageV2(HToupcam h, void* pImageData, int bits, ToupcamFrameInfoV2* pInfo);
MALLINCAM_API(HRESULT)  Toupcam_PullStillImageV2(HToupcam h, void* pImageData, int bits, ToupcamFrameInfoV2* pInfo);
MALLINCAM_API(HRESULT)  Toupcam_PullImageWithRowPitchV2(HToupcam h, void* pImageData, int bits, int rowPitch, ToupcamFrameInfoV2* pInfo);
MALLINCAM_API(HRESULT)  Toupcam_PullStillImageWithRowPitchV2(HToupcam h, void* pImageData, int bits, int rowPitch, ToupcamFrameInfoV2* pInfo);

MALLINCAM_API(HRESULT)  Toupcam_PullImage(HToupcam h, void* pImageData, int bits, unsigned* pnWidth, unsigned* pnHeight);
MALLINCAM_API(HRESULT)  Toupcam_PullStillImage(HToupcam h, void* pImageData, int bits, unsigned* pnWidth, unsigned* pnHeight);
MALLINCAM_API(HRESULT)  Toupcam_PullImageWithRowPitch(HToupcam h, void* pImageData, int bits, int rowPitch, unsigned* pnWidth, unsigned* pnHeight);
MALLINCAM_API(HRESULT)  Toupcam_PullStillImageWithRowPitch(HToupcam h, void* pImageData, int bits, int rowPitch, unsigned* pnWidth, unsigned* pnHeight);

/*
    (NULL == pData) means that something is error
    pCallbackCtx is the callback context which is passed by Toupcam_StartPushModeV3
    bSnap: TRUE if Toupcam_Snap

    pDataCallback is callbacked by an internal thread of mallincam.dll, so please pay attention to multithread problem.
    Do NOT call Toupcam_Close, Toupcam_Stop in this callback context, it deadlocks.
*/
typedef void (__stdcall* PMALLINCAM_DATA_CALLBACK_V3)(const void* pData, const ToupcamFrameInfoV2* pInfo, int bSnap, void* pCallbackCtx);
MALLINCAM_API(HRESULT)  Toupcam_StartPushModeV3(HToupcam h, PMALLINCAM_DATA_CALLBACK_V3 pDataCallback, void* pDataCallbackCtx, PMALLINCAM_EVENT_CALLBACK pEventCallback, void* pEventCallbackContext);

MALLINCAM_API(HRESULT)  Toupcam_Stop(HToupcam h);
MALLINCAM_API(HRESULT)  Toupcam_Pause(HToupcam h, int bPause);

/*  for pull mode: MALLINCAM_EVENT_STILLIMAGE, and then Toupcam_PullStillImage
    for push mode: the snapped image will be return by PMALLINCAM_DATA_CALLBACK(V2), with the parameter 'bSnap' set to 'TRUE'
*/
MALLINCAM_API(HRESULT)  Toupcam_Snap(HToupcam h, unsigned nResolutionIndex);  /* still image snap */
MALLINCAM_API(HRESULT)  Toupcam_SnapN(HToupcam h, unsigned nResolutionIndex, unsigned nNumber);  /* multiple still image snap */
/*
    soft trigger:
    nNumber:    0xffff:     trigger continuously
                0:          cancel trigger
                others:     number of images to be triggered
*/
MALLINCAM_API(HRESULT)  Toupcam_Trigger(HToupcam h, unsigned short nNumber);

/*
    put_Size, put_eSize, can be used to set the video output resolution BEFORE Toupcam_StartXXXX.
    put_Size use width and height parameters, put_eSize use the index parameter.
    for example, UCMOS03100KPA support the following resolutions:
            index 0:    2048,   1536
            index 1:    1024,   768
            index 2:    680,    510
    so, we can use put_Size(h, 1024, 768) or put_eSize(h, 1). Both have the same effect.
*/
MALLINCAM_API(HRESULT)  Toupcam_put_Size(HToupcam h, int nWidth, int nHeight);
MALLINCAM_API(HRESULT)  Toupcam_get_Size(HToupcam h, int* pWidth, int* pHeight);
MALLINCAM_API(HRESULT)  Toupcam_put_eSize(HToupcam h, unsigned nResolutionIndex);
MALLINCAM_API(HRESULT)  Toupcam_get_eSize(HToupcam h, unsigned* pnResolutionIndex);

/*
    final size after ROI, rotate, binning
*/
MALLINCAM_API(HRESULT)  Toupcam_get_FinalSize(HToupcam h, int* pWidth, int* pHeight);

MALLINCAM_API(HRESULT)  Toupcam_get_ResolutionNumber(HToupcam h);
MALLINCAM_API(HRESULT)  Toupcam_get_Resolution(HToupcam h, unsigned nResolutionIndex, int* pWidth, int* pHeight);
/*
    numerator/denominator, such as: 1/1, 1/2, 1/3
*/
MALLINCAM_API(HRESULT)  Toupcam_get_ResolutionRatio(HToupcam h, unsigned nResolutionIndex, int* pNumerator, int* pDenominator);
MALLINCAM_API(HRESULT)  Toupcam_get_Field(HToupcam h);

/*
see: http://www.fourcc.org
FourCC:
    MAKEFOURCC('G', 'B', 'R', 'G'), see http://www.siliconimaging.com/RGB%20Bayer.htm
    MAKEFOURCC('R', 'G', 'G', 'B')
    MAKEFOURCC('B', 'G', 'G', 'R')
    MAKEFOURCC('G', 'R', 'B', 'G')
    MAKEFOURCC('Y', 'Y', 'Y', 'Y'), monochromatic sensor
    MAKEFOURCC('Y', '4', '1', '1'), yuv411
    MAKEFOURCC('V', 'U', 'Y', 'Y'), yuv422
    MAKEFOURCC('U', 'Y', 'V', 'Y'), yuv422
    MAKEFOURCC('Y', '4', '4', '4'), yuv444
    MAKEFOURCC('R', 'G', 'B', '8'), RGB888

#ifndef MAKEFOURCC
#define MAKEFOURCC(a, b, c, d) ((unsigned)(unsigned char)(a) | ((unsigned)(unsigned char)(b) << 8) | ((unsigned)(unsigned char)(c) << 16) | ((unsigned)(unsigned char)(d) << 24))
#endif
*/
MALLINCAM_API(HRESULT)  Toupcam_get_RawFormat(HToupcam h, unsigned* nFourCC, unsigned* bitsperpixel);

/*
    ------------------------------------------------------------------|
    | Parameter               |   Range       |   Default             |
    |-----------------------------------------------------------------|
    | Auto Exposure Target    |   10~220      |   120                 |
    | Temp                    |   2000~15000  |   6503                |
    | Tint                    |   200~2500    |   1000                |
    | LevelRange              |   0~255       |   Low = 0, High = 255 |
    | Contrast                |   -100~100    |   0                   |
    | Hue                     |   -180~180    |   0                   |
    | Saturation              |   0~255       |   128                 |
    | Brightness              |   -64~64      |   0                   |
    | Gamma                   |   20~180      |   100                 |
    | WBGain                  |   -127~127    |   0                   |
    ------------------------------------------------------------------|
*/

#ifndef __MALLINCAM_CALLBACK_DEFINED__
#define __MALLINCAM_CALLBACK_DEFINED__
typedef void (__stdcall* PIMALLINCAM_EXPOSURE_CALLBACK)(void* pCtx);                                     /* auto exposure */
typedef void (__stdcall* PIMALLINCAM_WHITEBALANCE_CALLBACK)(const int aGain[3], void* pCtx);             /* once white balance, RGB Gain mode */
typedef void (__stdcall* PIMALLINCAM_BLACKBALANCE_CALLBACK)(const unsigned short aSub[3], void* pCtx);   /* once black balance */
typedef void (__stdcall* PIMALLINCAM_TEMPTINT_CALLBACK)(const int nTemp, const int nTint, void* pCtx);   /* once white balance, Temp/Tint Mode */
typedef void (__stdcall* PIMALLINCAM_HISTOGRAM_CALLBACK)(const float aHistY[256], const float aHistR[256], const float aHistG[256], const float aHistB[256], void* pCtx);
typedef void (__stdcall* PIMALLINCAM_CHROME_CALLBACK)(void* pCtx);
#endif

MALLINCAM_API(HRESULT)  Toupcam_get_AutoExpoEnable(HToupcam h, int* bAutoExposure);
MALLINCAM_API(HRESULT)  Toupcam_put_AutoExpoEnable(HToupcam h, int bAutoExposure);
MALLINCAM_API(HRESULT)  Toupcam_get_AutoExpoTarget(HToupcam h, unsigned short* Target);
MALLINCAM_API(HRESULT)  Toupcam_put_AutoExpoTarget(HToupcam h, unsigned short Target);

/*set the maximum/minimal auto exposure time and agin. The default maximum auto exposure time is 350ms */
MALLINCAM_API(HRESULT)  Toupcam_put_MaxAutoExpoTimeAGain(HToupcam h, unsigned maxTime, unsigned short maxAGain);
MALLINCAM_API(HRESULT)  Toupcam_get_MaxAutoExpoTimeAGain(HToupcam h, unsigned* maxTime, unsigned short* maxAGain);
MALLINCAM_API(HRESULT)  Toupcam_put_MinAutoExpoTimeAGain(HToupcam h, unsigned minTime, unsigned short minAGain);
MALLINCAM_API(HRESULT)  Toupcam_get_MinAutoExpoTimeAGain(HToupcam h, unsigned* minTime, unsigned short* minAGain);

MALLINCAM_API(HRESULT)  Toupcam_get_ExpoTime(HToupcam h, unsigned* Time); /* in microseconds */
MALLINCAM_API(HRESULT)  Toupcam_put_ExpoTime(HToupcam h, unsigned Time); /* in microseconds */
MALLINCAM_API(HRESULT)  Toupcam_get_RealExpoTime(HToupcam h, unsigned* Time); /* in microseconds, based on 50HZ/60HZ/DC */
MALLINCAM_API(HRESULT)  Toupcam_get_ExpTimeRange(HToupcam h, unsigned* nMin, unsigned* nMax, unsigned* nDef);

MALLINCAM_API(HRESULT)  Toupcam_get_ExpoAGain(HToupcam h, unsigned short* AGain); /* percent, such as 300 */
MALLINCAM_API(HRESULT)  Toupcam_put_ExpoAGain(HToupcam h, unsigned short AGain); /* percent */
MALLINCAM_API(HRESULT)  Toupcam_get_ExpoAGainRange(HToupcam h, unsigned short* nMin, unsigned short* nMax, unsigned short* nDef);

/* Auto White Balance "Once", Temp/Tint Mode */
MALLINCAM_API(HRESULT)  Toupcam_AwbOnce(HToupcam h, PIMALLINCAM_TEMPTINT_CALLBACK fnTTProc, void* pTTCtx); /* auto white balance "once". This function must be called AFTER Toupcam_StartXXXX */

/* Auto White Balance, RGB Gain Mode */
MALLINCAM_API(HRESULT)  Toupcam_AwbInit(HToupcam h, PIMALLINCAM_WHITEBALANCE_CALLBACK fnWBProc, void* pWBCtx);

/* White Balance, Temp/Tint mode */
MALLINCAM_API(HRESULT)  Toupcam_put_TempTint(HToupcam h, int nTemp, int nTint);
MALLINCAM_API(HRESULT)  Toupcam_get_TempTint(HToupcam h, int* nTemp, int* nTint);

/* White Balance, RGB Gain mode */
MALLINCAM_API(HRESULT)  Toupcam_put_WhiteBalanceGain(HToupcam h, int aGain[3]);
MALLINCAM_API(HRESULT)  Toupcam_get_WhiteBalanceGain(HToupcam h, int aGain[3]);

/* Black Balance */
MALLINCAM_API(HRESULT)  Toupcam_AbbOnce(HToupcam h, PIMALLINCAM_BLACKBALANCE_CALLBACK fnBBProc, void* pBBCtx); /* auto black balance "once". This function must be called AFTER Toupcam_StartXXXX */
MALLINCAM_API(HRESULT)  Toupcam_put_BlackBalance(HToupcam h, unsigned short aSub[3]);
MALLINCAM_API(HRESULT)  Toupcam_get_BlackBalance(HToupcam h, unsigned short aSub[3]);

/* Flat Field Correction */
MALLINCAM_API(HRESULT)  Toupcam_FfcOnce(HToupcam h);
#ifdef _WIN32
MALLINCAM_API(HRESULT)  Toupcam_FfcExport(HToupcam h, const wchar_t* filepath);
MALLINCAM_API(HRESULT)  Toupcam_FfcImport(HToupcam h, const wchar_t* filepath);
#else
MALLINCAM_API(HRESULT)  Toupcam_FfcExport(HToupcam h, const char* filepath);
MALLINCAM_API(HRESULT)  Toupcam_FfcImport(HToupcam h, const char* filepath);
#endif

/* Dark Field Correction */
MALLINCAM_API(HRESULT)  Toupcam_DfcOnce(HToupcam h);

#ifdef _WIN32
MALLINCAM_API(HRESULT)  Toupcam_DfcExport(HToupcam h, const wchar_t* filepath);
MALLINCAM_API(HRESULT)  Toupcam_DfcImport(HToupcam h, const wchar_t* filepath);
#else
MALLINCAM_API(HRESULT)  Toupcam_DfcExport(HToupcam h, const char* filepath);
MALLINCAM_API(HRESULT)  Toupcam_DfcImport(HToupcam h, const char* filepath);
#endif

MALLINCAM_API(HRESULT)  Toupcam_put_Hue(HToupcam h, int Hue);
MALLINCAM_API(HRESULT)  Toupcam_get_Hue(HToupcam h, int* Hue);
MALLINCAM_API(HRESULT)  Toupcam_put_Saturation(HToupcam h, int Saturation);
MALLINCAM_API(HRESULT)  Toupcam_get_Saturation(HToupcam h, int* Saturation);
MALLINCAM_API(HRESULT)  Toupcam_put_Brightness(HToupcam h, int Brightness);
MALLINCAM_API(HRESULT)  Toupcam_get_Brightness(HToupcam h, int* Brightness);
MALLINCAM_API(HRESULT)  Toupcam_get_Contrast(HToupcam h, int* Contrast);
MALLINCAM_API(HRESULT)  Toupcam_put_Contrast(HToupcam h, int Contrast);
MALLINCAM_API(HRESULT)  Toupcam_get_Gamma(HToupcam h, int* Gamma); /* percent */
MALLINCAM_API(HRESULT)  Toupcam_put_Gamma(HToupcam h, int Gamma);  /* percent */

MALLINCAM_API(HRESULT)  Toupcam_get_Chrome(HToupcam h, int* bChrome);  /* monochromatic mode */
MALLINCAM_API(HRESULT)  Toupcam_put_Chrome(HToupcam h, int bChrome);

MALLINCAM_API(HRESULT)  Toupcam_get_VFlip(HToupcam h, int* bVFlip);  /* vertical flip */
MALLINCAM_API(HRESULT)  Toupcam_put_VFlip(HToupcam h, int bVFlip);
MALLINCAM_API(HRESULT)  Toupcam_get_HFlip(HToupcam h, int* bHFlip);
MALLINCAM_API(HRESULT)  Toupcam_put_HFlip(HToupcam h, int bHFlip); /* horizontal flip */

MALLINCAM_API(HRESULT)  Toupcam_get_Negative(HToupcam h, int* bNegative);  /* negative film */
MALLINCAM_API(HRESULT)  Toupcam_put_Negative(HToupcam h, int bNegative);

MALLINCAM_API(HRESULT)  Toupcam_put_Speed(HToupcam h, unsigned short nSpeed);
MALLINCAM_API(HRESULT)  Toupcam_get_Speed(HToupcam h, unsigned short* pSpeed);
MALLINCAM_API(HRESULT)  Toupcam_get_MaxSpeed(HToupcam h); /* get the maximum speed, see "Frame Speed Level", the speed range = [0, max], closed interval */

MALLINCAM_API(HRESULT)  Toupcam_get_FanMaxSpeed(HToupcam h); /* get the maximum fan speed, the fan speed range = [0, max], closed interval */

MALLINCAM_API(HRESULT)  Toupcam_get_MaxBitDepth(HToupcam h); /* get the max bit depth of this camera, such as 8, 10, 12, 14, 16 */

/* power supply of lighting:
        0 -> 60HZ AC
        1 -> 50Hz AC
        2 -> DC
*/
MALLINCAM_API(HRESULT)  Toupcam_put_HZ(HToupcam h, int nHZ);
MALLINCAM_API(HRESULT)  Toupcam_get_HZ(HToupcam h, int* nHZ);

MALLINCAM_API(HRESULT)  Toupcam_put_Mode(HToupcam h, int bSkip); /* skip or bin */
MALLINCAM_API(HRESULT)  Toupcam_get_Mode(HToupcam h, int* bSkip); /* If the model don't support bin/skip mode, return E_NOTIMPL */

MALLINCAM_API(HRESULT)  Toupcam_put_AWBAuxRect(HToupcam h, const RECT* pAuxRect); /* auto white balance ROI */
MALLINCAM_API(HRESULT)  Toupcam_get_AWBAuxRect(HToupcam h, RECT* pAuxRect);
MALLINCAM_API(HRESULT)  Toupcam_put_AEAuxRect(HToupcam h, const RECT* pAuxRect);  /* auto exposure ROI */
MALLINCAM_API(HRESULT)  Toupcam_get_AEAuxRect(HToupcam h, RECT* pAuxRect);

MALLINCAM_API(HRESULT)  Toupcam_put_ABBAuxRect(HToupcam h, const RECT* pAuxRect); /* auto black balance ROI */
MALLINCAM_API(HRESULT)  Toupcam_get_ABBAuxRect(HToupcam h, RECT* pAuxRect);

/*
    S_FALSE:    color mode
    S_OK:       mono mode, such as EXCCD00300KMA and UHCCD01400KMA
*/
MALLINCAM_API(HRESULT)  Toupcam_get_MonoMode(HToupcam h);

MALLINCAM_API(HRESULT)  Toupcam_get_StillResolutionNumber(HToupcam h);
MALLINCAM_API(HRESULT)  Toupcam_get_StillResolution(HToupcam h, unsigned nResolutionIndex, int* pWidth, int* pHeight);

/*  0: stop grab frame when frame buffer deque is full, until the frames in the queue are pulled away and the queue is not full
    1: realtime
          use minimum frame buffer. When new frame arrive, drop all the pending frame regardless of whether the frame buffer is full.
          If DDR present, also limit the DDR frame buffer to only one frame.
    2: soft realtime
          Drop the oldest frame when the queue is full and then enqueue the new frame
    default: 0
*/
MALLINCAM_API(HRESULT)  Toupcam_put_RealTime(HToupcam h, int val);
MALLINCAM_API(HRESULT)  Toupcam_get_RealTime(HToupcam h, int* val);

/* discard the current internal frame cache.
    If DDR present, also discard the frames in the DDR.
    Toupcam_Flush is obsolete, it's a synonyms for Toupcam_Flush(h, MALLINCAM_OPTION_FLUSH, 3)
*/
MALLINCAM_DEPRECATED
MALLINCAM_API(HRESULT)  Toupcam_Flush(HToupcam h);

/* get the temperature of the sensor, in 0.1 degrees Celsius (32 means 3.2 degrees Celsius, -35 means -3.5 degree Celsius)
    return E_NOTIMPL if not supported
*/
MALLINCAM_API(HRESULT)  Toupcam_get_Temperature(HToupcam h, short* pTemperature);

/* set the target temperature of the sensor or TEC, in 0.1 degrees Celsius (32 means 3.2 degrees Celsius, -35 means -3.5 degree Celsius)
    return E_NOTIMPL if not supported
*/
MALLINCAM_API(HRESULT)  Toupcam_put_Temperature(HToupcam h, short nTemperature);

/*
    get the revision
*/
MALLINCAM_API(HRESULT)  Toupcam_get_Revision(HToupcam h, unsigned short* pRevision);

/*
    get the serial number which is always 32 chars which is zero-terminated such as "TP110826145730ABCD1234FEDC56787"
*/
MALLINCAM_API(HRESULT)  Toupcam_get_SerialNumber(HToupcam h, char sn[32]);

/*
    get the camera firmware version, such as: 3.2.1.20140922
*/
MALLINCAM_API(HRESULT)  Toupcam_get_FwVersion(HToupcam h, char fwver[16]);

/*
    get the camera hardware version, such as: 3.12
*/
MALLINCAM_API(HRESULT)  Toupcam_get_HwVersion(HToupcam h, char hwver[16]);

/*
    get the production date, such as: 20150327, YYYYMMDD, (YYYY: year, MM: month, DD: day)
*/
MALLINCAM_API(HRESULT)  Toupcam_get_ProductionDate(HToupcam h, char pdate[10]);

/*
    get the FPGA version, such as: 1.13
*/
MALLINCAM_API(HRESULT)  Toupcam_get_FpgaVersion(HToupcam h, char fpgaver[16]);

/*
    get the sensor pixel size, such as: 2.4um
*/
MALLINCAM_API(HRESULT)  Toupcam_get_PixelSize(HToupcam h, unsigned nResolutionIndex, float* x, float* y);

/* software level range */
MALLINCAM_API(HRESULT)  Toupcam_put_LevelRange(HToupcam h, unsigned short aLow[4], unsigned short aHigh[4]);
MALLINCAM_API(HRESULT)  Toupcam_get_LevelRange(HToupcam h, unsigned short aLow[4], unsigned short aHigh[4]);

/* hardware level range mode */
#define MALLINCAM_LEVELRANGE_MANUAL       0x0000  /* manual */
#define MALLINCAM_LEVELRANGE_ONCE         0x0001  /* once */
#define MALLINCAM_LEVELRANGE_CONTINUE     0x0002  /* continue */
#define MALLINCAM_LEVELRANGE_ROI          0xffff  /* update roi rect only */
MALLINCAM_API(HRESULT)  Toupcam_put_LevelRangeV2(HToupcam h, unsigned short mode, const RECT* pRoiRect, unsigned short aLow[4], unsigned short aHigh[4]);
MALLINCAM_API(HRESULT)  Toupcam_get_LevelRangeV2(HToupcam h, unsigned short* pMode, RECT* pRoiRect, unsigned short aLow[4], unsigned short aHigh[4]);

/*
    The following functions must be called AFTER Toupcam_StartPushMode or Toupcam_StartPullModeWithWndMsg or Toupcam_StartPullModeWithCallback
*/
MALLINCAM_API(HRESULT)  Toupcam_LevelRangeAuto(HToupcam h);  /* software level range */
MALLINCAM_API(HRESULT)  Toupcam_GetHistogram(HToupcam h, PIMALLINCAM_HISTOGRAM_CALLBACK fnHistogramProc, void* pHistogramCtx);

/* led state:
    iLed: Led index, (0, 1, 2, ...)
    iState: 1 -> Ever bright; 2 -> Flashing; other -> Off
    iPeriod: Flashing Period (>= 500ms)
*/
MALLINCAM_API(HRESULT)  Toupcam_put_LEDState(HToupcam h, unsigned short iLed, unsigned short iState, unsigned short iPeriod);

MALLINCAM_API(HRESULT)  Toupcam_write_EEPROM(HToupcam h, unsigned addr, const unsigned char* pBuffer, unsigned nBufferLen);
MALLINCAM_API(HRESULT)  Toupcam_read_EEPROM(HToupcam h, unsigned addr, unsigned char* pBuffer, unsigned nBufferLen);

MALLINCAM_API(HRESULT)  Toupcam_read_Pipe(HToupcam h, unsigned pipeNum, void* pBuffer, unsigned nBufferLen);
MALLINCAM_API(HRESULT)  Toupcam_write_Pipe(HToupcam h, unsigned pipeNum, const void* pBuffer, unsigned nBufferLen);
MALLINCAM_API(HRESULT)  Toupcam_feed_Pipe(HToupcam h, unsigned pipeNum);

#define MALLINCAM_TEC_TARGET_MIN               (-300)     /* -30.0 degrees Celsius */
#define MALLINCAM_TEC_TARGET_DEF               0          /* 0.0 degrees Celsius */
#define MALLINCAM_TEC_TARGET_MAX               300        /* 30.0 degrees Celsius */
                                             
#define MALLINCAM_OPTION_NOFRAME_TIMEOUT       0x01       /* no frame timeout: 1 = enable; 0 = disable. default: disable */
#define MALLINCAM_OPTION_THREAD_PRIORITY       0x02       /* set the priority of the internal thread which grab data from the usb device. iValue: 0 = THREAD_PRIORITY_NORMAL; 1 = THREAD_PRIORITY_ABOVE_NORMAL; 2 = THREAD_PRIORITY_HIGHEST; default: 0; see: msdn SetThreadPriority */
#define MALLINCAM_OPTION_PROCESSMODE           0x03       /* 0 = better image quality, more cpu usage. this is the default value
                                                           1 = lower image quality, less cpu usage
                                                        */
#define MALLINCAM_OPTION_RAW                   0x04       /* raw data mode, read the sensor "raw" data. This can be set only BEFORE Toupcam_StartXXX(). 0 = rgb, 1 = raw, default value: 0 */
#define MALLINCAM_OPTION_HISTOGRAM             0x05       /* 0 = only one, 1 = continue mode */
#define MALLINCAM_OPTION_BITDEPTH              0x06       /* 0 = 8 bits mode, 1 = 16 bits mode, subset of MALLINCAM_OPTION_PIXEL_FORMAT */
#define MALLINCAM_OPTION_FAN                   0x07       /* 0 = turn off the cooling fan, [1, max] = fan speed */
#define MALLINCAM_OPTION_TEC                   0x08       /* 0 = turn off the thermoelectric cooler, 1 = turn on the thermoelectric cooler */
#define MALLINCAM_OPTION_LINEAR                0x09       /* 0 = turn off the builtin linear tone mapping, 1 = turn on the builtin linear tone mapping, default value: 1 */
#define MALLINCAM_OPTION_CURVE                 0x0a       /* 0 = turn off the builtin curve tone mapping, 1 = turn on the builtin polynomial curve tone mapping, 2 = logarithmic curve tone mapping, default value: 2 */
#define MALLINCAM_OPTION_TRIGGER               0x0b       /* 0 = video mode, 1 = software or simulated trigger mode, 2 = external trigger mode, 3 = external + software trigger, default value = 0 */
#define MALLINCAM_OPTION_RGB                   0x0c       /* 0 => RGB24; 1 => enable RGB48 format when bitdepth > 8; 2 => RGB32; 3 => 8 Bits Gray (only for mono camera); 4 => 16 Bits Gray (only for mono camera when bitdepth > 8) */
#define MALLINCAM_OPTION_COLORMATIX            0x0d       /* enable or disable the builtin color matrix, default value: 1 */
#define MALLINCAM_OPTION_WBGAIN                0x0e       /* enable or disable the builtin white balance gain, default value: 1 */
#define MALLINCAM_OPTION_TECTARGET             0x0f       /* get or set the target temperature of the thermoelectric cooler, in 0.1 degree Celsius. For example, 125 means 12.5 degree Celsius, -35 means -3.5 degree Celsius */
#define MALLINCAM_OPTION_AUTOEXP_POLICY        0x10       /* auto exposure policy:
                                                            0: Exposure Only
                                                            1: Exposure Preferred
                                                            2: Gain Only
                                                            3: Gain Preferred
                                                            default value: 1
                                                        */
#define MALLINCAM_OPTION_FRAMERATE             0x11       /* limit the frame rate, range=[0, 63], the default value 0 means no limit */
#define MALLINCAM_OPTION_DEMOSAIC              0x12       /* demosaic method for both video and still image: BILINEAR = 0, VNG(Variable Number of Gradients interpolation) = 1, PPG(Patterned Pixel Grouping interpolation) = 2, AHD(Adaptive Homogeneity-Directed interpolation) = 3, see https://en.wikipedia.org/wiki/Demosaicing, default value: 0 */
#define MALLINCAM_OPTION_DEMOSAIC_VIDEO        0x13       /* demosaic method for video */
#define MALLINCAM_OPTION_DEMOSAIC_STILL        0x14       /* demosaic method for still image */
#define MALLINCAM_OPTION_BLACKLEVEL            0x15       /* black level */
#define MALLINCAM_OPTION_MULTITHREAD           0x16       /* multithread image processing */
#define MALLINCAM_OPTION_BINNING               0x17       /* binning, 0x01 (no binning), 0x02 (add, 2*2), 0x03 (add, 3*3), 0x04 (add, 4*4), 0x05 (add, 5*5), 0x06 (add, 6*6), 0x07 (add, 7*7), 0x08 (add, 8*8), 0x82 (average, 2*2), 0x83 (average, 3*3), 0x84 (average, 4*4), 0x85 (average, 5*5), 0x86 (average, 6*6), 0x87 (average, 7*7), 0x88 (average, 8*8). The final image size is rounded down to an even number, such as 640/3 to get 212 */
#define MALLINCAM_OPTION_ROTATE                0x18       /* rotate clockwise: 0, 90, 180, 270 */
#define MALLINCAM_OPTION_CG                    0x19       /* Conversion Gain: 0 = LCG, 1 = HCG, 2 = HDR */
#define MALLINCAM_OPTION_PIXEL_FORMAT          0x1a       /* pixel format, MALLINCAM_PIXELFORMAT_xxxx */
#define MALLINCAM_OPTION_FFC                   0x1b       /* flat field correction
                                                            set:
                                                                 0: disable
                                                                 1: enable
                                                                -1: reset
                                                                (0xff000000 | n): set the average number to n, [1~255]
                                                            get:
                                                                 (val & 0xff): 0 -> disable, 1 -> enable, 2 -> inited
                                                                 ((val & 0xff00) >> 8): sequence
                                                                 ((val & 0xff0000) >> 8): average number
                                                        */
#define MALLINCAM_OPTION_DDR_DEPTH             0x1c       /* the number of the frames that DDR can cache
                                                                1: DDR cache only one frame
                                                                0: Auto:
                                                                        ->one for video mode when auto exposure is enabled
                                                                        ->full capacity for others
                                                               -1: DDR can cache frames to full capacity
                                                        */
#define MALLINCAM_OPTION_DFC                   0x1d       /* dark field correction
                                                            set:
                                                                0: disable
                                                                1: enable
                                                               -1: reset
                                                                (0xff000000 | n): set the average number to n, [1~255]
                                                            get:
                                                                (val & 0xff): 0 -> disable, 1 -> enable, 2 -> inited
                                                                ((val & 0xff00) >> 8): sequence
                                                                ((val & 0xff0000) >> 8): average number
                                                        */
#define MALLINCAM_OPTION_SHARPENING            0x1e       /* Sharpening: (threshold << 24) | (radius << 16) | strength)
                                                            strength: [0, 500], default: 0 (disable)
                                                            radius: [1, 10]
                                                            threshold: [0, 255]
                                                        */
#define MALLINCAM_OPTION_FACTORY               0x1f       /* restore the factory settings */
#define MALLINCAM_OPTION_TEC_VOLTAGE           0x20       /* get the current TEC voltage in 0.1V, 59 mean 5.9V; readonly */
#define MALLINCAM_OPTION_TEC_VOLTAGE_MAX       0x21       /* get the TEC maximum voltage in 0.1V; readonly */
#define MALLINCAM_OPTION_DEVICE_RESET          0x22       /* reset usb device, simulate a replug */
#define MALLINCAM_OPTION_UPSIDE_DOWN           0x23       /* upsize down:
                                                            1: yes
                                                            0: no
                                                            default: 1 (win), 0 (linux/macos)
                                                        */
#define MALLINCAM_OPTION_AFPOSITION            0x24       /* auto focus sensor board positon */
#define MALLINCAM_OPTION_AFMODE                0x25       /* auto focus mode (0:manul focus; 1:auto focus; 2:once focus; 3:conjugate calibration) */
#define MALLINCAM_OPTION_AFZONE                0x26       /* auto focus zone */
#define MALLINCAM_OPTION_AFFEEDBACK            0x27       /* auto focus information feedback; 0:unknown; 1:focused; 2:focusing; 3:defocus; 4:up; 5:down */
#define MALLINCAM_OPTION_TESTPATTERN           0x28       /* test pattern:
                                                           0: TestPattern Off
                                                           3: monochrome diagonal stripes
                                                           5: monochrome vertical stripes
                                                           7: monochrome horizontal stripes
                                                           9: chromatic diagonal stripes
                                                        */
#define MALLINCAM_OPTION_AUTOEXP_THRESHOLD     0x29       /* threshold of auto exposure, default value: 5, range = [2, 15] */
#define MALLINCAM_OPTION_BYTEORDER             0x2a       /* Byte order, BGR or RGB: 0->RGB, 1->BGR, default value: 1(Win), 0(macOS, Linux, Android) */
#define MALLINCAM_OPTION_NOPACKET_TIMEOUT      0x2b       /* no packet timeout: 0 = disable, positive value = timeout milliseconds. default: disable */
#define MALLINCAM_OPTION_MAX_PRECISE_FRAMERATE 0x2c       /* precise frame rate maximum value in 0.1 fps, such as 115 means 11.5 fps */
#define MALLINCAM_OPTION_PRECISE_FRAMERATE     0x2d       /* precise frame rate current value in 0.1 fps */
#define MALLINCAM_OPTION_BANDWIDTH             0x2e       /* bandwidth, [1-100]% */
#define MALLINCAM_OPTION_RELOAD                0x2f       /* reload the last frame in trigger mode */
#define MALLINCAM_OPTION_CALLBACK_THREAD       0x30       /* dedicated thread for callback */
#define MALLINCAM_OPTION_FRAME_DEQUE_LENGTH    0x31       /* frame buffer deque length, range: [2, 1024], default: 3 */
#define MALLINCAM_OPTION_MIN_PRECISE_FRAMERATE 0x32       /* precise frame rate minimum value in 0.1 fps, such as 15 means 1.5 fps */
#define MALLINCAM_OPTION_SEQUENCER_ONOFF       0x33       /* sequencer trigger: on/off */
#define MALLINCAM_OPTION_SEQUENCER_NUMBER      0x34       /* sequencer trigger: number, range = [1, 255] */
#define MALLINCAM_OPTION_SEQUENCER_EXPOTIME    0x01000000 /* sequencer trigger: exposure time, iOption = MALLINCAM_OPTION_SEQUENCER_EXPOTIME | index, iValue = exposure time
                                                            For example, to set the exposure time of the third group to 50ms, call:
                                                               Toupcam_put_Option(MALLINCAM_OPTION_SEQUENCER_EXPOTIME | 3, 50000)
                                                        */
#define MALLINCAM_OPTION_SEQUENCER_EXPOGAIN    0x02000000 /* sequencer trigger: exposure gain, iOption = MALLINCAM_OPTION_SEQUENCER_EXPOGAIN | index, iValue = gain */
#define MALLINCAM_OPTION_DENOISE               0x35       /* denoise, strength range: [0, 100], 0 means disable */
#define MALLINCAM_OPTION_HEAT_MAX              0x36       /* get maximum level: heat to prevent fogging up */
#define MALLINCAM_OPTION_HEAT                  0x37       /* heat to prevent fogging up */
#define MALLINCAM_OPTION_LOW_NOISE             0x38       /* low noise mode (Higher signal noise ratio, lower frame rate): 1 => enable */
#define MALLINCAM_OPTION_POWER                 0x39       /* get power consumption, unit: milliwatt */
#define MALLINCAM_OPTION_GLOBAL_RESET_MODE     0x3a       /* global reset mode */
#define MALLINCAM_OPTION_OPEN_USB_ERRORCODE    0x3b       /* open usb error code */
#define MALLINCAM_OPTION_LINUX_USB_ZEROCOPY    0x3c       /* global option for linux platform:
                                                             enable or disable usb zerocopy (helps to reduce memory copy and improve efficiency. Requires kernel version >= 4.6 and hardware platform support)
                                                             if the image is wrong, this indicates that the hardware platform does not support this feature, please disable it when the program starts:
                                                               Toupcam_put_Option((this is a global option, the camera handle parameter is not required, use nullptr), MALLINCAM_OPTION_LINUX_USB_ZEROCOPY, 0)
                                                             default value:
                                                               disable(0): android or arm32
                                                               enable(1):  others
                                                        */
#define MALLINCAM_OPTION_FLUSH                 0x3d       /* 1 = hard flush, discard frames cached by camera DDR (if any)
                                                           2 = soft flush, discard frames cached by mallincam.dll (if any)
                                                           3 = both flush
                                                           Toupcam_Flush means 'both flush'
                                                        */

/* pixel format */
#define MALLINCAM_PIXELFORMAT_RAW8             0x00
#define MALLINCAM_PIXELFORMAT_RAW10            0x01
#define MALLINCAM_PIXELFORMAT_RAW12            0x02
#define MALLINCAM_PIXELFORMAT_RAW14            0x03
#define MALLINCAM_PIXELFORMAT_RAW16            0x04
#define MALLINCAM_PIXELFORMAT_YUV411           0x05
#define MALLINCAM_PIXELFORMAT_VUYY             0x06
#define MALLINCAM_PIXELFORMAT_YUV444           0x07
#define MALLINCAM_PIXELFORMAT_RGB888           0x08
#define MALLINCAM_PIXELFORMAT_GMCY8            0x09   /* map to RGGB 8 bits */
#define MALLINCAM_PIXELFORMAT_GMCY12           0x0a   /* map to RGGB 12 bits */
#define MALLINCAM_PIXELFORMAT_UYVY             0x0b

MALLINCAM_API(HRESULT)  Toupcam_put_Option(HToupcam h, unsigned iOption, int iValue);
MALLINCAM_API(HRESULT)  Toupcam_get_Option(HToupcam h, unsigned iOption, int* piValue);

/*
    xOffset, yOffset, xWidth, yHeight: must be even numbers
*/
MALLINCAM_API(HRESULT)  Toupcam_put_Roi(HToupcam h, unsigned xOffset, unsigned yOffset, unsigned xWidth, unsigned yHeight);
MALLINCAM_API(HRESULT)  Toupcam_get_Roi(HToupcam h, unsigned* pxOffset, unsigned* pyOffset, unsigned* pxWidth, unsigned* pyHeight);

/*  simulate replug:
    return > 0, the number of device has been replug
    return = 0, no device found
    return E_ACCESSDENIED if without UAC Administrator privileges
    for each device found, it will take about 3 seconds
*/
#ifdef _WIN32
MALLINCAM_API(HRESULT) Toupcam_Replug(const wchar_t* id);
#else
MALLINCAM_API(HRESULT) Toupcam_Replug(const char* id);
#endif

#ifndef __MALLINCAMAFPARAM_DEFINED__
#define __MALLINCAMAFPARAM_DEFINED__
typedef struct {
    int imax;    /* maximum auto focus sensor board positon */
    int imin;    /* minimum auto focus sensor board positon */
    int idef;    /* conjugate calibration positon */
    int imaxabs; /* maximum absolute auto focus sensor board positon, micrometer */
    int iminabs; /* maximum absolute auto focus sensor board positon, micrometer */
    int zoneh;   /* zone horizontal */
    int zonev;   /* zone vertical */
}ToupcamAfParam;
#endif

MALLINCAM_API(HRESULT)  Toupcam_get_AfParam(HToupcam h, ToupcamAfParam* pAfParam);

#define MALLINCAM_IOCONTROLTYPE_GET_SUPPORTEDMODE           0x01 /* 0x01->Input, 0x02->Output, (0x01 | 0x02)->support both Input and Output */
#define MALLINCAM_IOCONTROLTYPE_GET_GPIODIR                 0x03 /* 0x00->Input, 0x01->Output */
#define MALLINCAM_IOCONTROLTYPE_SET_GPIODIR                 0x04
#define MALLINCAM_IOCONTROLTYPE_GET_FORMAT                  0x05 /*
                                                                   0x00-> not connected
                                                                   0x01-> Tri-state: Tri-state mode (Not driven)
                                                                   0x02-> TTL: TTL level signals
                                                                   0x03-> LVDS: LVDS level signals
                                                                   0x04-> RS422: RS422 level signals
                                                                   0x05-> Opto-coupled
                                                               */
#define MALLINCAM_IOCONTROLTYPE_SET_FORMAT                  0x06
#define MALLINCAM_IOCONTROLTYPE_GET_OUTPUTINVERTER          0x07 /* boolean, only support output signal */
#define MALLINCAM_IOCONTROLTYPE_SET_OUTPUTINVERTER          0x08
#define MALLINCAM_IOCONTROLTYPE_GET_INPUTACTIVATION         0x09 /* 0x00->Positive, 0x01->Negative */
#define MALLINCAM_IOCONTROLTYPE_SET_INPUTACTIVATION         0x0a
#define MALLINCAM_IOCONTROLTYPE_GET_DEBOUNCERTIME           0x0b /* debouncer time in microseconds, [0, 20000] */
#define MALLINCAM_IOCONTROLTYPE_SET_DEBOUNCERTIME           0x0c
#define MALLINCAM_IOCONTROLTYPE_GET_TRIGGERSOURCE           0x0d /*
                                                                  0x00-> Opto-isolated input
                                                                  0x01-> GPIO0
                                                                  0x02-> GPIO1
                                                                  0x03-> Counter
                                                                  0x04-> PWM
                                                                  0x05-> Software
                                                               */
#define MALLINCAM_IOCONTROLTYPE_SET_TRIGGERSOURCE           0x0e
#define MALLINCAM_IOCONTROLTYPE_GET_TRIGGERDELAY            0x0f /* Trigger delay time in microseconds, [0, 5000000] */
#define MALLINCAM_IOCONTROLTYPE_SET_TRIGGERDELAY            0x10
#define MALLINCAM_IOCONTROLTYPE_GET_BURSTCOUNTER            0x11 /* Burst Counter: 1, 2, 3 ... 1023 */
#define MALLINCAM_IOCONTROLTYPE_SET_BURSTCOUNTER            0x12
#define MALLINCAM_IOCONTROLTYPE_GET_COUNTERSOURCE           0x13 /* 0x00-> Opto-isolated input, 0x01-> GPIO0, 0x02-> GPIO1 */
#define MALLINCAM_IOCONTROLTYPE_SET_COUNTERSOURCE           0x14
#define MALLINCAM_IOCONTROLTYPE_GET_COUNTERVALUE            0x15 /* Counter Value: 1, 2, 3 ... 1023 */
#define MALLINCAM_IOCONTROLTYPE_SET_COUNTERVALUE            0x16
#define MALLINCAM_IOCONTROLTYPE_SET_RESETCOUNTER            0x18
#define MALLINCAM_IOCONTROLTYPE_GET_PWM_FREQ                0x19
#define MALLINCAM_IOCONTROLTYPE_SET_PWM_FREQ                0x1a
#define MALLINCAM_IOCONTROLTYPE_GET_PWM_DUTYRATIO           0x1b
#define MALLINCAM_IOCONTROLTYPE_SET_PWM_DUTYRATIO           0x1c
#define MALLINCAM_IOCONTROLTYPE_GET_PWMSOURCE               0x1d /* 0x00-> Opto-isolated input, 0x01-> GPIO0, 0x02-> GPIO1 */
#define MALLINCAM_IOCONTROLTYPE_SET_PWMSOURCE               0x1e
#define MALLINCAM_IOCONTROLTYPE_GET_OUTPUTMODE              0x1f /*
                                                                  0x00-> Frame Trigger Wait
                                                                  0x01-> Exposure Active
                                                                  0x02-> Strobe
                                                                  0x03-> User output
                                                               */
#define MALLINCAM_IOCONTROLTYPE_SET_OUTPUTMODE              0x20
#define MALLINCAM_IOCONTROLTYPE_GET_STROBEDELAYMODE         0x21 /* boolean, 0-> pre-delay, 1-> delay; compared to exposure active signal */
#define MALLINCAM_IOCONTROLTYPE_SET_STROBEDELAYMODE         0x22
#define MALLINCAM_IOCONTROLTYPE_GET_STROBEDELAYTIME         0x23 /* Strobe delay or pre-delay time in microseconds, [0, 5000000] */
#define MALLINCAM_IOCONTROLTYPE_SET_STROBEDELAYTIME         0x24
#define MALLINCAM_IOCONTROLTYPE_GET_STROBEDURATION          0x25 /* Strobe duration time in microseconds, [0, 5000000] */
#define MALLINCAM_IOCONTROLTYPE_SET_STROBEDURATION          0x26
#define MALLINCAM_IOCONTROLTYPE_GET_USERVALUE               0x27 /* 
                                                                  bit0-> Opto-isolated output
                                                                  bit1-> GPIO0 output
                                                                  bit2-> GPIO1 output
                                                               */
#define MALLINCAM_IOCONTROLTYPE_SET_USERVALUE               0x28
#define MALLINCAM_IOCONTROLTYPE_GET_UART_ENABLE             0x29 /* enable: 1-> on; 0-> off */
#define MALLINCAM_IOCONTROLTYPE_SET_UART_ENABLE             0x2a
#define MALLINCAM_IOCONTROLTYPE_GET_UART_BAUDRATE           0x2b /* baud rate: 0-> 9600; 1-> 19200; 2-> 38400; 3-> 57600; 4-> 115200 */
#define MALLINCAM_IOCONTROLTYPE_SET_UART_BAUDRATE           0x2c
#define MALLINCAM_IOCONTROLTYPE_GET_UART_LINEMODE           0x2d /* line mode: 0-> TX(GPIO_0)/RX(GPIO_1); 1-> TX(GPIO_1)/RX(GPIO_0) */
#define MALLINCAM_IOCONTROLTYPE_SET_UART_LINEMODE           0x2e

#define MALLINCAM_IOCONTROL_DELAYTIME_MAX                   (5 * 1000 * 1000)

MALLINCAM_API(HRESULT)  Toupcam_IoControl(HToupcam h, unsigned index, unsigned nType, int outVal, int* inVal);

MALLINCAM_API(HRESULT)  Toupcam_write_UART(HToupcam h, const unsigned char* pData, unsigned nDataLen);
MALLINCAM_API(HRESULT)  Toupcam_read_UART(HToupcam h, unsigned char* pBuffer, unsigned nBufferLen);

MALLINCAM_API(HRESULT)  Toupcam_put_Linear(HToupcam h, const unsigned char* v8, const unsigned short* v16);
MALLINCAM_API(HRESULT)  Toupcam_put_Curve(HToupcam h, const unsigned char* v8, const unsigned short* v16);
MALLINCAM_API(HRESULT)  Toupcam_put_ColorMatrix(HToupcam h, const double v[9]);
MALLINCAM_API(HRESULT)  Toupcam_put_InitWBGain(HToupcam h, const unsigned short v[3]);

/*
    get the frame rate: framerate (fps) = Frame * 1000.0 / nTime
*/
MALLINCAM_API(HRESULT)  Toupcam_get_FrameRate(HToupcam h, unsigned* nFrame, unsigned* nTime, unsigned* nTotalFrame);

/* astronomy: for ST4 guide, please see: ASCOM Platform Help ICameraV2.
    nDirect: 0 = North, 1 = South, 2 = East, 3 = West, 4 = Stop
    nDuration: in milliseconds
*/
MALLINCAM_API(HRESULT)  Toupcam_ST4PlusGuide(HToupcam h, unsigned nDirect, unsigned nDuration);

/* S_OK: ST4 pulse guiding
   S_FALSE: ST4 not pulse guiding
*/
MALLINCAM_API(HRESULT)  Toupcam_ST4PlusGuideState(HToupcam h);

/*
    calculate the clarity factor:
    pImageData: pointer to the image data
    bits: 8(Grey), 24 (RGB24), 32(RGB32)
    nImgWidth, nImgHeight: the image width and height
*/
MALLINCAM_API(double)   Toupcam_calc_ClarityFactor(const void* pImageData, int bits, unsigned nImgWidth, unsigned nImgHeight);

/*
    nBitCount: output bitmap bit count
    when nBitDepth == 8:
        nBitCount must be 24 or 32
    when nBitDepth > 8
        nBitCount:  24 -> RGB24
                    32 -> RGB32
                    48 -> RGB48
                    64 -> RGB64
*/
MALLINCAM_API(void)     Toupcam_deBayerV2(unsigned nBayer, int nW, int nH, const void* input, void* output, unsigned char nBitDepth, unsigned char nBitCount);

/*
    obsolete, please use Toupcam_deBayerV2
*/
MALLINCAM_DEPRECATED
MALLINCAM_API(void)     Toupcam_deBayer(unsigned nBayer, int nW, int nH, const void* input, void* output, unsigned char nBitDepth);

typedef void (__stdcall* PMALLINCAM_DEMOSAIC_CALLBACK)(unsigned nBayer, int nW, int nH, const void* input, void* output, unsigned char nBitDepth, void* pCallbackCtx);
MALLINCAM_API(HRESULT)  Toupcam_put_Demosaic(HToupcam h, PMALLINCAM_DEMOSAIC_CALLBACK pCallback, void* pCallbackCtx);

/*
    obsolete, please use ToupcamModelV2
*/
typedef struct {
#ifdef _WIN32
    const wchar_t*      name;       /* model name, in Windows, we use unicode */
#else
    const char*         name;       /* model name */
#endif
    unsigned            flag;       /* MALLINCAM_FLAG_xxx */
    unsigned            maxspeed;   /* number of speed level, same as Toupcam_get_MaxSpeed(), the speed range = [0, maxspeed], closed interval */
    unsigned            preview;    /* number of preview resolution, same as Toupcam_get_ResolutionNumber() */
    unsigned            still;      /* number of still resolution, same as Toupcam_get_StillResolutionNumber() */
    ToupcamResolution   res[MALLINCAM_MAX];
}ToupcamModel; /* camera model */

/*
    obsolete, please use ToupcamDeviceV2
*/
typedef struct {
#ifdef _WIN32
    wchar_t             displayname[64];    /* display name */
    wchar_t             id[64];             /* unique and opaque id of a connected camera, for Toupcam_Open */
#else
    char                displayname[64];    /* display name */
    char                id[64];             /* unique and opaque id of a connected camera, for Toupcam_Open */
#endif
    const ToupcamModel* model;
}ToupcamDevice; /* camera instance for enumerating */

/*
    obsolete, please use Toupcam_EnumV2
*/
MALLINCAM_DEPRECATED
MALLINCAM_API(unsigned) Toupcam_Enum(ToupcamDevice pti[MALLINCAM_MAX]);

typedef PMALLINCAM_DATA_CALLBACK_V3 PMALLINCAM_DATA_CALLBACK_V2;
MALLINCAM_DEPRECATED
MALLINCAM_API(HRESULT)  Toupcam_StartPushModeV2(HToupcam h, PMALLINCAM_DATA_CALLBACK_V2 pDataCallback, void* pCallbackCtx);

typedef void (__stdcall* PMALLINCAM_DATA_CALLBACK)(const void* pData, const BITMAPINFOHEADER* pHeader, int bSnap, void* pCallbackCtx);
MALLINCAM_DEPRECATED
MALLINCAM_API(HRESULT)  Toupcam_StartPushMode(HToupcam h, PMALLINCAM_DATA_CALLBACK pDataCallback, void* pCallbackCtx);

MALLINCAM_DEPRECATED
MALLINCAM_API(HRESULT)  Toupcam_put_ExpoCallback(HToupcam h, PIMALLINCAM_EXPOSURE_CALLBACK fnExpoProc, void* pExpoCtx);
MALLINCAM_DEPRECATED
MALLINCAM_API(HRESULT)  Toupcam_put_ChromeCallback(HToupcam h, PIMALLINCAM_CHROME_CALLBACK fnChromeProc, void* pChromeCtx);

/* Toupcam_FfcOnePush is obsolete, it's a synonyms for Toupcam_FfcOnce. */
MALLINCAM_DEPRECATED
MALLINCAM_API(HRESULT)  Toupcam_FfcOnePush(HToupcam h);

/* Toupcam_DfcOnePush is obsolete, it's a synonyms for Toupcam_DfcOnce. */
MALLINCAM_DEPRECATED
MALLINCAM_API(HRESULT)  Toupcam_DfcOnePush(HToupcam h);

/* Toupcam_AwbOnePush is obsolete, it's a synonyms for Toupcam_AwbOnce. */
MALLINCAM_DEPRECATED
MALLINCAM_API(HRESULT)  Toupcam_AwbOnePush(HToupcam h, PIMALLINCAM_TEMPTINT_CALLBACK fnTTProc, void* pTTCtx);

/* Toupcam_AbbOnePush is obsolete, it's a synonyms for Toupcam_AbbOnce. */
MALLINCAM_DEPRECATED
MALLINCAM_API(HRESULT)  Toupcam_AbbOnePush(HToupcam h, PIMALLINCAM_BLACKBALANCE_CALLBACK fnBBProc, void* pBBCtx);

#ifndef _WIN32

/*
This function is only available on macOS and Linux, it's unnecessary on Windows.
  (1) To process the device plug in / pull out in Windows, please refer to the MSDN
       (a) Device Management, http://msdn.microsoft.com/en-us/library/windows/desktop/aa363224(v=vs.85).aspx
       (b) Detecting Media Insertion or Removal, http://msdn.microsoft.com/en-us/library/windows/desktop/aa363215(v=vs.85).aspx
  (2) To process the device plug in / pull out in Linux / macOS, please call this function to register the callback function.
      When the device is inserted or pulled out, you will be notified by the callback funcion, and then call Toupcam_EnumV2(...) again to enum the cameras.
Recommendation: for better rubustness, when notify of device insertion arrives, don't open handle of this device immediately, but open it after delaying a short time (e.g., 200 milliseconds).
*/
typedef void (*PMALLINCAM_HOTPLUG)(void* pCallbackCtx);
MALLINCAM_API(void)   Toupcam_HotPlug(PMALLINCAM_HOTPLUG pHotPlugCallback, void* pCallbackCtx);

#else

/* Toupcam_Start is obsolete, it's a synonyms for Toupcam_StartPushMode. */
MALLINCAM_DEPRECATED
MALLINCAM_API(HRESULT)  Toupcam_Start(HToupcam h, PMALLINCAM_DATA_CALLBACK pDataCallback, void* pCallbackCtx);

/* Toupcam_put_TempTintInit is obsolete, it's a synonyms for Toupcam_AwbOnce. */
MALLINCAM_DEPRECATED
MALLINCAM_API(HRESULT)  Toupcam_put_TempTintInit(HToupcam h, PIMALLINCAM_TEMPTINT_CALLBACK fnTTProc, void* pTTCtx);

/*
    obsolete, please use Toupcam_put_Option or Toupcam_get_Option to set or get the process mode: MALLINCAM_PROCESSMODE_FULL or MALLINCAM_PROCESSMODE_FAST.
    default is MALLINCAM_PROCESSMODE_FULL.
*/
#ifndef __MALLINCAM_PROCESSMODE_DEFINED__
#define __MALLINCAM_PROCESSMODE_DEFINED__
#define MALLINCAM_PROCESSMODE_FULL        0x00    /* better image quality, more cpu usage. this is the default value */
#define MALLINCAM_PROCESSMODE_FAST        0x01    /* lower image quality, less cpu usage */
#endif

MALLINCAM_DEPRECATED
MALLINCAM_API(HRESULT)  Toupcam_put_ProcessMode(HToupcam h, unsigned nProcessMode);
MALLINCAM_DEPRECATED
MALLINCAM_API(HRESULT)  Toupcam_get_ProcessMode(HToupcam h, unsigned* pnProcessMode);

#endif

/* obsolete, please use Toupcam_put_Roi and Toupcam_get_Roi */
MALLINCAM_DEPRECATED
MALLINCAM_API(HRESULT)  Toupcam_put_RoiMode(HToupcam h, int bRoiMode, int xOffset, int yOffset);
MALLINCAM_DEPRECATED
MALLINCAM_API(HRESULT)  Toupcam_get_RoiMode(HToupcam h, int* pbRoiMode, int* pxOffset, int* pyOffset);

/* obsolete:
     ------------------------------------------------------------|
     | Parameter         |   Range       |   Default             |
     |-----------------------------------------------------------|
     | VidgetAmount      |   -100~100    |   0                   |
     | VignetMidPoint    |   0~100       |   50                  |
     -------------------------------------------------------------
*/
MALLINCAM_API(HRESULT)  Toupcam_put_VignetEnable(HToupcam h, int bEnable);
MALLINCAM_API(HRESULT)  Toupcam_get_VignetEnable(HToupcam h, int* bEnable);
MALLINCAM_API(HRESULT)  Toupcam_put_VignetAmountInt(HToupcam h, int nAmount);
MALLINCAM_API(HRESULT)  Toupcam_get_VignetAmountInt(HToupcam h, int* nAmount);
MALLINCAM_API(HRESULT)  Toupcam_put_VignetMidPointInt(HToupcam h, int nMidPoint);
MALLINCAM_API(HRESULT)  Toupcam_get_VignetMidPointInt(HToupcam h, int* nMidPoint);

/* obsolete flags */
#define MALLINCAM_FLAG_BITDEPTH10    MALLINCAM_FLAG_RAW10  /* pixel format, RAW 10bits */
#define MALLINCAM_FLAG_BITDEPTH12    MALLINCAM_FLAG_RAW12  /* pixel format, RAW 12bits */
#define MALLINCAM_FLAG_BITDEPTH14    MALLINCAM_FLAG_RAW14  /* pixel format, RAW 14bits */
#define MALLINCAM_FLAG_BITDEPTH16    MALLINCAM_FLAG_RAW16  /* pixel format, RAW 16bits */

#ifdef _WIN32
MALLINCAM_API(HRESULT)  Toupcam_put_Name(const wchar_t* id, const char* name);
MALLINCAM_API(HRESULT)  Toupcam_get_Name(const wchar_t* id, char name[64]);
#else
MALLINCAM_API(HRESULT)  Toupcam_put_Name(const char* id, const char* name);
MALLINCAM_API(HRESULT)  Toupcam_get_Name(const char* id, char name[64]);
#endif
MALLINCAM_API(unsigned) Toupcam_EnumWithName(ToupcamDeviceV2 pti[MALLINCAM_MAX]);

#ifdef _WIN32
#pragma pack(pop)
#endif

#ifdef __cplusplus
}
#endif

#endif
