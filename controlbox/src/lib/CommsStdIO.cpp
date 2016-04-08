#include "CommsStdIO.h"

#if !CONTROLBOX_WIRING

#if defined(WIN32)
static int is_pipe = 0;
static HANDLE input_handle = 0;
#endif

StdIO::StdIO()
: in(std::cin), out(stdout)
{
#if defined(WIN32)
    DWORD dw;
  input_handle = GetStdHandle(STD_INPUT_HANDLE);
  is_pipe = !GetConsoleMode(input_handle, &dw);
#endif
}

void StdIO::print(char c) {
    fputc(c,out);
}
void StdIO::print(const char* c) {
    fputs(c,out);
}
void StdIO::printNewLine() {
    fputs("\r\n", out);
    flush();
}
void StdIO::println() {
    printNewLine();
}

#if 0
#if defined(WIN32)
int input_available()
{
  DWORD nchars;
  /* When using Standard C input functions, also check if there
   is anything in the buffer. After a call to such functions,
   the input waiting in the pipe will be copied to the buffer,
   and the call to PeekNamedPipe can indicate no input available.
   Setting stdin to unbuffered was not enough, IIRC */
  if (stdin->_cnt > 0)
    return 1;
  if (is_pipe)
  {
    /* When running under a GUI, you will end here. */
    if (!PeekNamedPipe(input_handle, NULL, 0, NULL, &nchars, NULL))
      /* Something went wrong. Probably the parent program exited.
         Could call exit() here. Returning 1 will make the next call
         to the input function return EOF, where this should be
         caught then. */
      return 1;

    return (nchars != 0);
  }
  else
    return _kbhit() != 0; /* In "text-mode" without GUI */
}

int StdIO::available() {
    return input_available();
}
#else

#include <stdio.h>
#include <sys/poll.h>

int StdIO::available() {
    struct pollfd fds;
     fds.fd = 0; /* this is STDIN */
     fds.events = POLLIN;
     fds.revents = 0;
     int ret = poll(&fds, 1, 0);
     if (ret > 0 && (fds.revents & POLLIN))
        return 1;
     else
        return 0;
}
#endif

#endif

int StdIO::available() {
    return in.available();
}

void StdIO::begin(unsigned long)
{
}

size_t StdIO::write(uint8_t w) {
    fputc(w, out);
    fflush(out);
    return 1;
}

int StdIO::read() {
    return in.next();
}

void StdIO::flush() { fflush(out); }

int StdIO::peek() {
	return in.peek();
}

template<> void StreamDataOut<StdIO>::close() {
}

template<> bool StreamDataIn<StdIO>::hasNext() {
    return stream;
}

#endif
