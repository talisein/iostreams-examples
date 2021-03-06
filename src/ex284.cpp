/*
  author:  "Klaus Wittlich" <Klaus_Wittlich@sae.de>

  Based on source code published in the book "Standard C++ IOStreams
  and Locales" by Angelika Langer & Klaus Kreft, Copyright (c) 2000 by
  Addison Wesley Longman, Inc.

  Permission to use, copy, and modify this software for any non-profit
  purpose is hereby granted without fee.  Neither the author of this
  source code, Klaus Wittlich, nor the authors of the above mentioned
  book, Angelika Langer and Klaus Kreft, nor the publisher, Addison
  Wesley Longman, Inc., make any representations about the suitability of this
  software for any purpose.  It is provided "as is" without express or
  implied warranty.
*/

#include <locale>
#include <cassert>
#include <fcntl.h>
#include <sys/stat.h>
#include <iostream>
#include <unistd.h>
#include <cstring>

using namespace ::std;

const char *testdata_filename = "ex284.dat";

// We first need to create some example data
void CreateDataFile()
{
    char text[] = "abcdefghij";
    int fd = open(testdata_filename, O_WRONLY | O_CREAT, S_IWRITE | S_IREAD);
    assert(fd >= 0);
    write(fd,text,sizeof(text));
    close(fd);
}

typedef mbstate_t ConversionState; // We use the default type

enum {
    NoError,
    NoMoreInput,
    ReadError,
    ResultBufferFull,
    ConversionError
};


int main(int, char *[])
{
    CreateDataFile();

    const int fd = open(testdata_filename, O_RDONLY);
    if (0 > fd) perror("open()");
    unlink(testdata_filename);
    assert(fd >= 0);

    // Ensure file will be closed later when exiting
    struct CloseFd {
        CloseFd(const int& fd) : mfd(fd) {};
        ~CloseFd() {
            close(mfd);
        }
        const int& mfd;
    } CloseFd(fd);

    locale loc("C");

    // p. 284 {{{
    ConversionState cs;

    const int transBufSize  = 32;
    char transBuf[transBufSize];
    int readSize            = transBufSize;
    char * readStart        = transBuf;

    const int resultBufSize = 1024;
    wchar_t resultBuf[resultBufSize];
    wchar_t *to             = resultBuf;
    wchar_t *toLimit        = resultBuf + resultBufSize;

    int err                 = 0;

    while ( !err )
    {
        const char * fromNext;
        wchar_t *toNext;
        int readResult;
        codecvt_base::result convResult;

        if ((readResult = read (fd, readStart, readSize)) <= 0)
        {
            if (readResult == 0)
                err = NoMoreInput;
            else
                err = ReadError;
            break;
        }

        convResult = use_facet< codecvt<wchar_t, char, ConversionState> >(loc).in
            (cs, transBuf, transBuf + transBufSize, fromNext, to, toLimit, toNext);
// }}}
// p. 285 {{{
        to = toNext;

        if (to == toLimit)
            err =ResultBufferFull;
        else if(convResult == codecvt_base::error)
            err = ConversionError;
        else if(convResult == codecvt_base::ok)
        {
            readSize = transBufSize;
            readStart = transBuf;
        }
        else if ( convResult == codecvt_base::partial )
        {
            int num = transBuf + transBufSize - fromNext;
            char * untransFrom = transBuf + (fromNext - transBuf);
            copy(untransFrom, transBuf + transBufSize, transBuf);
            readSize = transBufSize - num;
            readStart = transBuf + num;
        }
    }
// }}}
    write(1, resultBuf, (to - resultBuf) * sizeof(wchar_t));
//    cout << endl;
//    wcout << L"The converted file is:" << endl << resultBuf << endl;
//    wcout << resultBuf << endl;
    return 0;
}
/* Input: No input
 */
/* Output:
   The converted file is:
   abcdefghij
*/
