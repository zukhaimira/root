//===-- SystemZSubtarget.h - SystemZ subtarget information -----*- C++ -*--===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file declares the SystemZ specific subclass of TargetSubtargetInfo.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_SYSTEMZ_SYSTEMZSUBTARGET_H
#define LLVM_LIB_TARGET_SYSTEMZ_SYSTEMZSUBTARGET_H

#include "SystemZFrameLowering.h"
#include "SystemZISelLowering.h"
#include "SystemZInstrInfo.h"
#include "SystemZRegisterInfo.h"
#include "SystemZSelectionDAGInfo.h"
#include "llvm/IR/DataLayout.h"
#include "llvm/ADT/Triple.h"
#include "llvm/Target/TargetSubtargetInfo.h"
#include <string>

#define GET_SUBTARGETINFO_HEADER
#include "SystemZGenSubtargetInfo.inc"

namespace llvm {
class GlobalValue;
class StringRef;

class SystemZSubtarget : public SystemZGenSubtargetInfo {
  virtual void anchor();
protected:
  bool HasDistinctOps;
  bool HasLoadStoreOnCond;
  bool HasHighWord;
  bool HasFPExtension;
  bool HasFastSerialization;
  bool HasInterlockedAccess1;

private:
  Triple TargetTriple;
  SystemZInstrInfo InstrInfo;
  SystemZTargetLowering TLInfo;
  SystemZSelectionDAGInfo TSInfo;
  SystemZFrameLowering FrameLowering;

  SystemZSubtarget &initializeSubtargetDependencies(StringRef CPU,
                                                    StringRef FS);
public:
  SystemZSubtarget(const std::string &TT, const std::string &CPU,
                   const std::string &FS, const TargetMachine &TM);

  const TargetFrameLowering *getFrameLowering() const override {
    return &FrameLowering;
  }
  const SystemZInstrInfo *getInstrInfo() const override { return &InstrInfo; }
  const SystemZRegisterInfo *getRegisterInfo() const override {
    return &InstrInfo.getRegisterInfo();
  }
  const SystemZTargetLowering *getTargetLowering() const override {
    return &TLInfo;
  }
  const TargetSelectionDAGInfo *getSelectionDAGInfo() const override {
    return &TSInfo;
  }

  // This is important for reducing register pressure in vector code.
  bool useAA() const override { return true; }

  // Automatically generated by tblgen.
  void ParseSubtargetFeatures(StringRef CPU, StringRef FS);

  // Return true if the target has the distinct-operands facility.
  bool hasDistinctOps() const { return HasDistinctOps; }

  // Return true if the target has the load/store-on-condition facility.
  bool hasLoadStoreOnCond() const { return HasLoadStoreOnCond; }

  // Return true if the target has the high-word facility.
  bool hasHighWord() const { return HasHighWord; }

  // Return true if the target has the floating-point extension facility.
  bool hasFPExtension() const { return HasFPExtension; }

  // Return true if the target has the fast-serialization facility.
  bool hasFastSerialization() const { return HasFastSerialization; }

  // Return true if the target has interlocked-access facility 1.
  bool hasInterlockedAccess1() const { return HasInterlockedAccess1; }

  // Return true if GV can be accessed using LARL for reloc model RM
  // and code model CM.
  bool isPC32DBLSymbol(const GlobalValue *GV, Reloc::Model RM,
                       CodeModel::Model CM) const;

  bool isTargetELF() const { return TargetTriple.isOSBinFormatELF(); }
};
} // end namespace llvm

#endif
