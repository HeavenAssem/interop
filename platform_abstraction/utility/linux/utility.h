//
// Created by islam on 11.05.17.
//

#pragma once


#include "definitions.h"

#include <errno.h>


std::string_view read_errno() {
    switch (errno) {
        case E2BIG:
            return "argument list too long";
        case EACCES:
            return "permission denied";
        case EADDRINUSE:
            return "address already in use";
        case EADDRNOTAVAIL:
            return "address not available";
        case EAFNOSUPPORT:
            return "address family not supported";
        case EAGAIN:
            return "resource temporarily unavailable";
        case EALREADY:
            return "connection already in progress";
        case EBADE:
            return "bad exchange";
        case EBADF:
            return "bad file descriptor";
        case EBADFD:
            return "file descriptor in bad state";
        case EBADMSG:
            return "bad message";
        case EBADR:
            return "invalid request descriptor";
        case EBADRQC:
            return "invalid request code";
        case EBADSLT:
            return "invalid slot";
        case EBUSY:
            return "device or resource busy";
        case ECANCELED:
            return "operation cancelled";
        case ECHILD:
            return "no child processes";
        case ECHRNG:
            return "channel number out of range";
        case ECOMM:
            return "communication error on send";
        case ECONNABORTED:
            return "connection aborted";
        case ECONNREFUSED:
            return "connection refused";
        case ECONNRESET:
            return "connection reset";
        case EDEADLOCK:
            return "resource deadlock avoided";
        case EDESTADDRREQ:
            return "destination address required";
        case EDOM:
            return "mathematics argument out of domain of function";
        case EDQUOT:
            return "disk quota exceeded";
        case EEXIST:
            return "file exists";
        case EFAULT:
            return "bad address";
        case EFBIG:
            return "file too large";
        case EHOSTDOWN:
            return "host is down";
        case EHOSTUNREACH:
            return "host is unreachable";
        case EIDRM:
            return "identifier removed";
        case EILSEQ:
            return "invalid or incomplete multibyte or wide character (illegal byte sequence)";
        case EINPROGRESS:
            return "operation in progress";
        case EINTR:
            return "interrupted function call";
        case EINVAL:
            return "invalid argument";
        case EIO:
            return "I/O error";
        case EISCONN:
            return "socket is connected";
        case EISDIR:
            return "is a directory";
        case EISNAM:
            return "is a named type file";
        case EKEYEXPIRED:
            return "key has expired";
        case EKEYREJECTED:
            return "key was rejected by service";
        case EKEYREVOKED:
            return "key has been revoked";
        case EL2HLT:
            return "level 2 halted";
        case EL2NSYNC:
            return "level 2 not synchronized";
        case EL3HLT:
            return "level 3 halted";
        case EL3RST:
            return "level 3 reset";
        case ELIBACC:
            return "cannot access a needed shared library";
        case ELIBBAD:
            return "accessing a corrupted shared library";
        case ELIBMAX:
            return "attempting to link in too many shared libraries";
        case ELIBSCN:
            return ".lib section corrupted";
        case ELIBEXEC:
            return "cannot exec a shared library directly";
        case ELOOP:
            return "too many levels of symbolic links";
        case EMEDIUMTYPE:
            return "wrong medium type";
        case EMFILE:
            return "too many file descriptors in use by process";
        case EMLINK:
            return "too many links";
        case EMSGSIZE:
            return "message too long";
        case EMULTIHOP:
            return "multihop attempted";
        case ENAMETOOLONG:
            return "filename too long";
        case ENETDOWN:
            return "network is down";
        case ENETRESET:
            return "connection aborted by network";
        case ENETUNREACH:
            return "network unreachable";
        case ENFILE:
            return "too many files are currently open in the system";
        case ENOBUFS:
            return "no buffer space available";
        case ENODATA:
            return "no message is available on the STREAM head read queue";
        case ENODEV:
            return "no such device";
        case ENOENT:
            return "no such file or directory";
        case ENOEXEC:
            return "exec format error";
        case ENOKEY:
            return "required key not available";
        case ENOLCK:
            return "no locks available";
        case ENOLINK:
            return "link has been severed";
        case ENOMEDIUM:
            return "no medium found";
        case ENOMEM:
            return "insufficient memory to complete the operation";
        case ENOMSG:
            return "no message of the desired type";
        case ENONET:
            return "machine is not on the network";
        case ENOPKG:
            return "package not installed";
        case ENOPROTOOPT:
            return "protocol not available";
        case ENOSPC:
            return "no space left on device";
        case ENOSR:
            return "no STREAM resources";
        case ENOSTR:
            return "not a STREAM";
        case ENOSYS:
            return "function not implemented";
        case ENOTBLK:
            return "block device required";
        case ENOTCONN:
            return "the socket is not connected";
        case ENOTDIR:
            return "is not a directory";
        case ENOTEMPTY:
            return "directory not empty";
        case ENOTSOCK:
            return "not a socket";
        case ENOTSUP:
            return "operation not supported";
        case ENOTTY:
            return "inappropriate I/O control operation";
        case ENOTUNIQ:
            return "name not unique on network";
        case ENXIO:
            return "no such device or address";
        case EOVERFLOW:
            return "value too large to be stored in data type";
        case EPERM:
            return "operation not permitted";
        case EPFNOSUPPORT:
            return "protocol family not supported";
        case EPIPE:
            return "broken pipe";
        case EPROTO:
            return "protocol error";
        case EPROTONOSUPPORT:
            return "protocol not supported";
        case EPROTOTYPE:
            return "protocol wrong type for socket";
        case ERANGE:
            return "result too large";
        case EREMCHG:
            return "remote address changed";
        case EREMOTE:
            return "object is remote";
        case EREMOTEIO:
            return "remote I/O error";
        case ERESTART:
            return "interrupted system call should be restarted";
        case EROFS:
            return "read-only filesystem";
        case ESHUTDOWN:
            return "cannot send after transport endpoint shutdown";
        case ESPIPE:
            return "invalid seek";
        case ESOCKTNOSUPPORT:
            return "socket type not supported";
        case ESRCH:
            return "no such process";
        case ESTALE:
            return "stale file handle";
        case ESTRPIPE:
            return "streams pipe error";
        case ETIME:
            return "timer expired";
        case ETIMEDOUT:
            return "connection timed out";
        case ETXTBSY:
            return "text file busy";
        case EUCLEAN:
            return "structure needs cleaning";
        case EUNATCH:
            return "protocol driver not attached";
        case EUSERS:
            return "too many users";
        case EXDEV:
            return "improper link";
        case EXFULL:
            return "exchange full";
        default:
            return "unknown error";
    }
}
