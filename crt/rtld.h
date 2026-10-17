/* Copyright (C) 2026 John Törnblom

This program is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the
Free Software Foundation; either version 3, or (at your option) any
later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; see the file COPYING. If not, see
<http://www.gnu.org/licenses/>.  */


/**
 *
 **/
#define DT_NULL    0
#define DT_NEEDED  1
#define DT_STRTAB  5


/**
 *
 **/
typedef struct {
  long d_tag;
  union {
    unsigned long d_val;
    unsigned long d_ptr;
  } d_un;
} Elf64_Dyn;


/**
 * Dependencies provided by the ELF linker.
 **/
extern const Elf64_Dyn _DYNAMIC[] __attribute__((visibility("hidden")));
extern const char __image_base[] __attribute__((visibility("hidden")));


/**
 * Dependencies provided by libkernel.sprx
 **/
extern int sceKernelDebugOutText(int channel, const char *text);
extern int sceKernelDlsym(int, const char*, void*);
extern const char* sceKernelGetFsSandboxRandomWord(void);
extern int sceKernelLoadStartModule(const char*, unsigned long, const void*, unsigned int, void*, int*);
extern int sceKernelStopUnloadModule(int, unsigned long, const void*, unsigned int, const void*, void*);
extern int stat(void*, const char*);


/**
 * Dependencies provided by libSceLibcInternal.sprx
 **/
extern int sprintf(char *str, const char *format, ...);
extern int snprintf(char *str, unsigned long n, const char *format, ...);
extern int vsnprintf(char *str, unsigned long n, const char *format, __builtin_va_list ap);
extern int strcmp(const char *s1, const char *s2);
extern char *strerror(int errnum);
extern int* __error(void);


/**
 * Dependencies provided by libSceSysmodule.sprx
 **/
extern int sceSysmoduleLoadModuleInternal(unsigned int);


/**
 * Lookup table for sceSysmoduleLoadModuleInternal().
 **/
static struct sysmodtab {
  const char* name;
  unsigned int handle;
} sysmodtab[] = {
  {"libSceAbstractLocal.sprx", 0x8000005f},
  {"libSceAbstractStorage.sprx", 0x80000058},
  {"libSceAbstractTcs.sprx", 0x800000a1},
  {"libSceAbstractTwitter.sprx", 0x80000062},
  {"libSceAbstractYoutube.sprx", 0x80000061},
  {"libSceAc3Enc.sprx", 0x80000013},
  {"libSceAjm.native.sprx", 0x80000023},
  {"libSceAjmi.sprx", 0x8000007e},
  {"libSceAmpr.sprx", 0x800000b7},
  {"libSceAppChecker.sprx", 0x80000032},
  {"libSceAppDbShellCoreClient.sprx", 0x800000a7},
  {"libSceAppInstUtil.sprx", 0x80000014},
  {"libSceAsyncStorageInternal.sprx", 0x80000077},
  {"libSceAudioIn.sprx", 0x80000002},
  {"libSceAudioOut.sprx", 0x80000001},
  {"libSceAudioSystem.sprx", 0x80000083},
  {"libSceAudiodecCpuDtsHdMa.sprx", 0x8000002d},
  {"libSceAudiodecCpuLpcm.sprx", 0x8000002e},
  {"libSceAudiodecCpuTrhd.sprx", 0x80000082},
  {"libSceAvSetting.sprx", 0x80000021},
  {"libSceAvcap2.sprx", 0x80000085},
  {"libSceBackupRestoreUtil.sprx", 0x8000003f},
  {"libSceBgft.sprx", 0x8000002a},
  {"libSceBgsStorage.sprx", 0x800000a3},
  {"libSceCamera.sprx", 0x8000001a},
  {"libSceCdlgUtilServer.sprx", 0x80000007},
  {"libSceCommonDialog.sprx", 0x80000018},
  {"libSceComposite.sprx", 0x8000008a},
  {"libSceCompositeExt.sprx", 0x8000008b},
  {"libSceContentListController.sprx", 0x800000ad},
  {"libSceDataTransfer.sprx", 0x80000057},
  {"libSceDbg.sprx", 0x80000025},
  {"libSceDipsw.sprx", 0x80000029},
  {"libSceDseehx.sprx", 0x80000056},
  {"libSceDtsEnc.sprx", 0x80000028},
  {"libSceEmbeddedTts.sprx", 0x8000009c},
  {"libSceEmbeddedTtsCoreG3.sprx", 0x8000009b},
  {"libSceFsInternalForVsh.sprx", 0x80000066},
  {"libSceGLSlimVSH.sprx", 0x800000a9},
  {"libSceGifParser.sprx", 0x8000005e},
  {"libSceGpuCapture.sprx", 0x8000007f},
  {"libSceGpuTrace.sprx", 0x8000007b},
  {"libSceGvMp4Parser.sprx", 0x8000005c},
  {"libSceHidControl.sprx", 0x80000017},
  {"libSceHttp.sprx", 0x8000000a},
  {"libSceHttp2.sprx", 0x8000008c},
  {"libSceHttpCache.sprx", 0x80000078},
  {"libSceIcu.sprx", 0x800000a8},
  {"libSceIdu.sprx", 0x800000a6},
  {"libSceImageUtil.sprx", 0x80000059},
  {"libSceIpmi.sprx", 0x8000001d},
  {"libSceJemspace.sprx", 0x8000009e},
  {"libSceJitBridge.sprx", 0x8000006f},
  {"libSceJpegParser.sprx", 0x8000005b},
  {"libSceJsc.sprx", 0x800000b0},
  {"libSceJscCompiler.sprx", 0x80000070},
  {"libSceJxr.sprx", 0x800000b4},
  {"libSceJxrParser.sprx", 0x800000b5},
  {"libSceKbEmulate.sprx", 0x80000031},
  {"libSceLibreSsl.sprx", 0x80000065},
  {"libSceLibreSsl3.sprx", 0x800000b8},
  {"libSceLoginMgrServer.sprx", 0x80000045},
  {"libSceMarlin.sprx", 0x80000027},
  {"libSceMat.sprx", 0x80000048},
  {"libSceMbus.sprx", 0x8000001e},
  {"libSceMediaFrameworkInterface.sprx", 0x80000095},
  {"libSceMediaFrameworkUtil.sprx", 0x800000b6},
  {"libSceMetadataReaderWriter.sprx", 0x8000005a},
  {"libSceNKWeb.sprx", 0x80000079},
  {"libSceNKWebKit.sprx", 0x8000007a},
  {"libSceNet.sprx", 0x8000001c},
  {"libSceNetCtl.sprx", 0x80000009},
  {"libSceNgs2.sprx", 0x80000090},
  {"libSceNpCommon.sprx", 0x8000000c},
  {"libSceNpGameIntent.sprx", 0x8000008d},
  {"libSceNpManager.sprx", 0x8000000d},
  {"libSceNpRemotePlaySessionSignaling.sprx", 0x8000009a},
  {"libSceNpSns.sprx", 0x8000001b},
  {"libSceNpTcs.sprx", 0x800000a0},
  {"libSceNpWebApi.sprx", 0x8000000e},
  {"libSceNpWebApi2.sprx", 0x8000008f},
  {"libSceOpusCeltDec.sprx", 0x80000044},
  {"libSceOpusCeltEnc.sprx", 0x80000043},
  {"libSceOpusDec.sprx", 0x80000069},
  {"libSceOpusSilkEnc.sprx", 0x80000068},
  {"libSceOrbisCompat.sprx", 0x80000071},
  {"libScePad.sprx", 0x80000024},
  {"libScePngParser.sprx", 0x8000005d},
  {"libScePosixForWebKit.sprx", 0x80000098},
  {"libScePsm.sprx", 0x80000030},
  {"libSceRazorCpu_debug.sprx", 0x80000075},
  {"libSceRegMgr.sprx", 0x8000001f},
  {"libSceRemotePlayClientIpc.sprx", 0x800000b9},
  {"libSceResourceArbitrator.sprx", 0x80000092},
  {"libSceRnpsAppMgr.sprx", 0x80000076},
  {"libSceRtc.sprx", 0x80000020},
  {"libSceSaveData.sprx", 0x8000000f},
  {"libSceShareInternal.native.sprx", 0x8000008e},
  {"libSceSocialScreen.sprx", 0x800000ae},
  {"libSceSsl.sprx", 0x8000000b},
  {"libSceSulphaDrv.sprx", 0x8000003b},
  {"libSceSysCore.sprx", 0x80000004},
  {"libSceSysUtil.sprx", 0x80000026},
  {"libSceSystemLogger2.sprx", 0x800000b3},
  {"libSceSystemLogger2Delivery.sprx", 0x80000089},
  {"libSceSystemLogger2Game.sprx", 0x8000009f},
  {"libSceSystemLogger2NativeQueueClient.sprx", 0x80000088},
  {"libSceSystemService.sprx", 0x80000010},
  {"libSceSystemTts.sprx", 0x80000097},
  {"libSceTEEClient.sprx", 0x800000a2},
  {"libSceUserService.sprx", 0x80000011},
  {"libSceVcodec.sprx", 0x80000091},
  {"libSceVdecCore.native.sprx", 0x80000015},
  {"libSceVdecSavc2.native.sprx", 0x80000036},
  {"libSceVdecShevc.native.sprx", 0x8000003c},
  {"libSceVdecSvp9.native.sprx", 0x800000af},
  {"libSceVenc.sprx", 0x80000084},
  {"libSceVideoOut.sprx", 0x80000022},
  {"libSceVideoOutSecondary.sprx", 0x80000046},
  {"libSceVideoStreamingEngine_sys.sprx", 0x800000b2},
  {"libSceVisionManager.sprx", 0x80000012},
  {"libSceVnaInternal.sprx", 0x8000007c},
  {"libSceVnaWebsocket.sprx", 0x8000007d},
  {"libSceVoiceCommand.sprx", 0x80000099},
  {"libSceWeb.sprx", 0x80000072},
  {"libSceWebKit2.sprx", 0x80000073},
  {"libSceWebKit2Secure.sprx", 0x80000074},
  {"libSceWebmParserMdrw.sprx", 0x800000a4},
  {"libcairo.sprx", 0x800000ac},
  {"libcurl.sprx", 0x800000b1},
  {"libicu.sprx", 0x800000aa},
  {"libpng16.sprx", 0x800000ab}
};


/**
 *
 **/
static int
rtld_klog(const char *fmt, ...) {
  char buf[0x200] = "<118>[rtld] ";
  __builtin_va_list args;


  __builtin_va_start(args, fmt);
  vsnprintf(buf+12, sizeof(buf) - 13, fmt, args);
  __builtin_va_end(args);

  return sceKernelDebugOutText(0, buf);
}


/**
 * Resolve the full path to a library filename.
 **/
static int
rtld_resolve_filename(const char *name, char* path, unsigned long size) {
  static const char* const system_dirs[] = {
    "/system/priv/lib",
    "/system/common/lib",
    "/system_ex/priv_ex/lib",
    "/system_ex/common_ex/lib",
  };
  static const char* const sandbox_dirs[] = {
    "priv/lib",
    "common/lib",
    "priv_ex/lib",
    "common_ex/lib",
  };
  const char* random_word;
  char buf[0x100] = {0};

  for(int i=0; i<sizeof(system_dirs)/sizeof(system_dirs[0]); i++) {
    snprintf(path, size, "%s/%s", system_dirs[i], name);
    if(!stat(path, buf)) {
      return 0;
    }
  }

  if((random_word=sceKernelGetFsSandboxRandomWord())) {
    for(int i=0; i<sizeof(sandbox_dirs)/sizeof(sandbox_dirs[0]); i++) {
      snprintf(path, size, "/%s/%s/%s", random_word, sandbox_dirs[i], name);
      if(!stat(path, buf)) {
        return 0;
      }
    }
  }

  snprintf(path, size, "/app0/sce_module/%s", name);
  if(!stat(path, buf)) {
    return 0;
  }

  snprintf(path, size, "%s", name);
  if(!stat(path, buf)) {
    return 0;
  }

  return -1;
}


/**
 * Whether a library is a system module that libSceSysmodule initializes.
 **/
static int
rtld_is_sysmodule(const char* filename) {
  for(int i=0; i<sizeof(sysmodtab)/sizeof(sysmodtab[0]); i++) {
    if(!strcmp(filename, sysmodtab[i].name)) {
      return 1;
    }
  }
  return 0;
}


/**
 * Initialize a library with the given filename.
 **/
static int
rtld_init_needed(const char* filename) {
  char path[1024];
  int err;

  // some sprx files don't need to be initialized
  if(!strcmp(filename, "libkernel.sprx") ||
     !strcmp(filename, "libkernel_web.sprx") ||
     !strcmp(filename, "libkernel_sys.sprx") ||
     !strcmp(filename, "libSceLibcInternal.sprx") ||
     !strcmp(filename, "libSceSysmodule.sprx")) {
    return 0;
  }

  if(rtld_resolve_filename(filename, path, 1024)) {
    rtld_klog("Unable to find %s\n", filename);
    return -1;
  }

  for(int i=0; i<sizeof(sysmodtab)/sizeof(sysmodtab[0]); i++) {
    if(!strcmp(filename, sysmodtab[i].name)) {
      if((err=sceSysmoduleLoadModuleInternal(sysmodtab[i].handle)) < 0) {
	rtld_klog("sceSysmoduleLoadModuleInternal: 0x%08x\n", err);
	return -1;
      }
    }
  }

  if((err=sceKernelLoadStartModule(path, 0, 0, 0, 0, 0)) < 0) {
    rtld_klog("sceKernelLoadStartModule: 0x%08x\n", err);
    return -1;
  }

  return 0;
}


/**
 * Initialize all libraries this module (the executable or a homebrew
 * library) depends on, as listed by the DT_NEEDED entries of its own dynamic
 * table.  The kernel does not load a library's dependencies, so every module
 * calls this when it starts: loading a homebrew library starts it, and it
 * then loads its own dependencies in turn.
 *
 * System modules are initialized first, whatever their order in DT_NEEDED.
 **/
static int
rtld_init(void) {
  char* strtab = 0;

  // find lookup tables
  for(int i=0; _DYNAMIC[i].d_tag!=DT_NULL; i++) {
    switch(_DYNAMIC[i].d_tag) {
    case DT_STRTAB:
      strtab = (char*)(__image_base + _DYNAMIC[i].d_un.d_ptr);
      break;
    }
  }

  // load needed libraries: system modules first, then the others
  for(int pass=0; pass<2; pass++) {
    for(int i=0; _DYNAMIC[i].d_tag!=DT_NULL; i++) {
      const char* filename;

      if(_DYNAMIC[i].d_tag != DT_NEEDED) {
	continue;
      }

      filename = strtab + _DYNAMIC[i].d_un.d_val;
      if(rtld_is_sysmodule(filename) != (pass == 0)) {
	continue;
      }

      if(rtld_init_needed(filename)) {
	return -1;
      }
    }
  }

  return 0;
}
