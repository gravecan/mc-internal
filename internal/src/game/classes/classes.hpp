#pragma once

class c_classes
{
public:
	void load( );
	void log( );
	void shutdown( );

	c_jni_ref<jclass> mc_class;
	c_jni_ref<jclass> level_class;
	c_jni_ref<jclass> config_class;
	c_jni_ref<jclass> living_class;
	c_jni_ref<jclass> text_class;
	c_jni_ref<jclass> world_class;
	c_jni_ref<jclass> player_class;
	c_jni_ref<jclass> rtc_class;

	std::atomic<bool> uhhthisisfordebugging = true;
	std::thread uhhthisisfordebugging_thread;
};

extern c_classes* classes;