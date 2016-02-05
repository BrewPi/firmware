
#include "application.h"
#include "unit-test/unit-test.h"

#if PLATFORM_THREADING
test(Thread_creation)
{
    volatile bool threadRan = false;
    Thread testThread = Thread("test", [&]() {
        threadRan = true;
        for(;;) {}
    });

    for(int tries = 5; !threadRan && tries >= 0; tries--) {
        delay(1);
    }

    testThread.dispose();

    assertTrue((bool)threadRan);
}

test(thread_SingleThreadedBlock)
{
	SINGLE_THREADED_BLOCK() {

	}
	SINGLE_THREADED_BLOCK() {

	}
}

test(thread_with_lock)
{
	WITH_LOCK(Serial) {

	}

	WITH_LOCK(Serial) {

	}

}

test(thread_try_lock)
{
	TRY_LOCK(Serial) {

	}
}

// todo - test for SingleThreadedSection


#endif
