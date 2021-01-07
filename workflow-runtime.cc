#include <nan.h>
#include <isolated_vm.h>

#include <iostream>
#include <chrono>
#include <thread>

using namespace isolated_vm;
using namespace v8;

struct TimeoutCallback : public Runnable {
	RemoteHandle<Context> context;
	RemoteHandle<Function> fn;

	TimeoutCallback(
		RemoteHandle<Context> context,
		RemoteHandle<Function> fn
	) :
		context(std::move(context)),
		fn(std::move(fn)) {}

	void Run() override {
		Context::Scope context_scope(*context);
		Local<Function> local_fn = *fn;
		Local<Value> argv[0];
		Nan::Call(local_fn, Local<Object>::Cast(Nan::Undefined()), 0, argv);
	}
};

NAN_METHOD(timeout) {
	IsolateHolder isolate_holder = IsolateHolder::GetCurrent();
	RemoteHandle<Context> context(Isolate::GetCurrent()->GetCurrentContext());
	RemoteHandle<Function> fn(Local<Function>::Cast(info[0]));

	uint32_t ms = Nan::To<uint32_t>(info[1]).FromJust();
	std::thread timeout_thread([=]() mutable {
		// Note that in this closure it is not safe to call into The only thing you can do is
		// schedule a task.
        std::cout << "set timer: " << ms << std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(ms));
        std::cout << "called after: " << ms << std::endl;
		isolate_holder.ScheduleTask(std::make_unique<TimeoutCallback>(std::move(context), std::move(fn)));
	});
	timeout_thread.detach();

	info.GetReturnValue().Set(Nan::Undefined());
}

ISOLATED_VM_MODULE void InitForContext(Isolate* isolate, Local<Context> context, Local<Object> target) {
	Nan::Set(target, Nan::New("timeout").ToLocalChecked(), Nan::GetFunction(Nan::New<FunctionTemplate>(timeout)).ToLocalChecked());
}

NAN_MODULE_INIT(init) {
	Isolate* isolate = Isolate::GetCurrent();
	InitForContext(isolate, isolate->GetCurrentContext(), target);
}
NODE_MODULE(native, init);
