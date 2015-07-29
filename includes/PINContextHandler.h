#ifndef PINCONTEXTHANDLER_H
#define PINCONTEXTHANDLER_H

#include "pin.H"

#include "ContextHandler.h"
#include "Registers.h"
#include "types.h"


class PINContextHandler: public ContextHandler {
  public:
    PINContextHandler(CONTEXT *ctx, THREADID threadId);

    uint64   getMemValue(uint64 mem, uint32 readSize) const;
    uint64   getSSERegisterValue(uint64 regID) const;
    uint32    getThreadID(void) const;
    uint64    getFlagValue(uint64 TritFlagID) const;
    uint64    getRegisterValue(uint64 regID) const;
    void      *getCtx(void) const;
    void      setMemValue(uint64 mem, uint32 readSize, uint64 value) const;
    void      setRegisterValue(uint64 TritRegID, uint64 value) const;
    void      setSSERegisterValue(uint64 TritRegID, uint64 value) const;

  private:
    CONTEXT   *_ctx;
    THREADID  _threadId;
};

#endif // PINCONTEXTHANDLER_H
