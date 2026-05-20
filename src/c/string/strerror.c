#include <stddef.h>

static const char *resolve_err_text(int code)
{
    switch (code) {
    case 0:
        return "Success";
    case 1:
        return "Action prohibited";
    case 2:
        return "Entity not found";
    case 3:
        return "Process lookup failed";
    case 4:
        return "System call interrupted";
    case 5:
        return "I/O failure";
    case 6:
        return "Device not configured";
    case 7:
        return "Argument list too big";
    case 8:
        return "Invalid executable format";
    case 9:
        return "Invalid file descriptor";
    case 10:
        return "No child processes found";
    case 11:
        return "Deadlock prevented";
    case 12:
        return "Out of memory";
    case 13:
        return "Access forbidden";
    case 14:
        return "Invalid memory address";
    case 15:
        return "Block device required";
    case 16:
        return "Device in use";
    case 17:
        return "File already exists";
    case 18:
        return "Link across devices";
    case 19:
        return "Device does not support operation";
    case 20:
        return "Path is not a directory";
    case 21:
        return "Path is a directory";
    case 22:
        return "Invalid argument provided";
    case 23:
        return "System file limit reached";
    case 24:
        return "Process file limit reached";
    case 25:
        return "Inappropriate IOCTL";
    case 26:
        return "Executable file is busy";
    case 27:
        return "File size too large";
    case 28:
        return "No space on device";
    case 29:
        return "Illegal seek operation";
    case 30:
        return "Filesystem is read-only";
    case 31:
        return "Too many links";
    case 32:
        return "Broken pipe";
    case 33:
        return "Argument out of domain";
    case 34:
        return "Result out of range";
    case 35:
        return "Resource unavailable try again";
    case 36:
        return "Operation in progress";
    case 37:
        return "Operation currently proceeding";
    case 38:
        return "Not a socket";
    case 39:
        return "Dest address needed";
    case 40:
        return "Message too huge";
    case 41:
        return "Protocol wrong type";
    case 42:
        return "Protocol unavailable";
    case 43:
        return "Protocol unsupported";
    case 44:
        return "Socket type unsupported";
    case 45:
        return "Not supported";
    case 46:
        return "Protocol family unsupported";
    case 47:
        return "Addr family unsupported";
    case 48:
        return "Address in use";
    case 49:
        return "Cannot assign address";
    case 50:
        return "Network down";
    case 51:
        return "Network unreachable";
    case 52:
        return "Network reset connection";
    case 53:
        return "Connection aborted locally";
    case 54:
        return "Connection reset by peer";
    case 55:
        return "Buffer space exhausted";
    case 56:
        return "Socket connected";
    case 57:
        return "Socket not connected";
    case 58:
        return "Socket shutdown cannot send";
    case 59:
        return "Too many refs";
    case 60:
        return "Request timed out";
    case 61:
        return "Connection refused";
    case 62:
        return "Symlink loop detected";
    case 63:
        return "Filename too long";
    case 64:
        return "Host down";
    case 65:
        return "No route to host";
    case 66:
        return "Directory not empty";
    case 67:
        return "Process limit exceeded";
    case 68:
        return "User limit exceeded";
    case 69:
        return "Quota exceeded";
    case 70:
        return "Stale file handle";
    case 71:
        return "Remote path too deep";
    case 72:
        return "Bad RPC structure";
    case 73:
        return "RPC version mismatch";
    case 74:
        return "RPC program unavailable";
    case 75:
        return "Program version mismatch";
    case 76:
        return "Bad RPC procedure";
    case 77:
        return "No locks available";
    case 78:
        return "Not implemented";
    case 79:
        return "Wrong file type";
    case 80:
        return "Auth error";
    case 81:
        return "Auth required";
    case 82:
        return "Power off";
    case 83:
        return "Device error";
    case 84:
        return "Value overflow";
    case 85:
        return "Bad executable";
    case 86:
        return "Wrong CPU type";
    case 87:
        return "Library version mismatch";
    case 88:
        return "Corrupt Mach-O";
    case 89:
        return "Canceled";
    case 90:
        return "ID removed";
    case 91:
        return "No message";
    case 92:
        return "Illegal byte seq";
    case 93:
        return "Attr not found";
    case 94:
        return "Bad message";
    case 95:
        return "Reserved (EMULTIHOP)";
    case 96:
        return "No data on stream";
    case 97:
        return "Reserved (ENOLINK)";
    case 98:
        return "No stream resources";
    case 99:
        return "Not a stream";
    case 100:
        return "Protocol error";
    case 101:
        return "Stream timeout";
    case 102:
        return "Op not supported on socket";
    case 103:
        return "No policy";
    case 104:
        return "Not recoverable";
    case 105:
        return "Owner died";
    case 106:
        return "Output queue full";
    default:
        return NULL;
    }
}

static void util_strcpy(char *dst, const char *src, size_t cap)
{
    size_t i = 0;
    if (cap == 0)
        return;

    while (src[i] != '\0' && i < cap - 1) {
        dst[i] = src[i];
        i++;
    }
    dst[i] = '\0';
}

static void format_unknown(int code, char *buf, size_t cap)
{
    unsigned int val;
    const char *base_msg = "Unknown error: ";
    size_t i = 0;
    char temp_num[32];
    int t_idx = 0;

    while (base_msg[i] && i < cap - 1) {
        buf[i] = base_msg[i];
        i++;
    }

    if (i >= cap - 1) {
        buf[i] = '\0';
        return;
    }

    if (code < 0) {
        buf[i++] = '-';
        val = (unsigned int)(-code);
    } else {
        val = (unsigned int)code;
    }

    if (i >= cap - 1) {
        buf[i] = '\0';
        return;
    }

    if (val == 0) {
        temp_num[t_idx++] = '0';
    } else {
        while (val > 0) {
            temp_num[t_idx++] = (val % 10) + '0';
            val /= 10;
        }
    }

    while (t_idx > 0 && i < cap - 1) {
        buf[i++] = temp_num[--t_idx];
    }

    buf[i] = '\0';
}

char *strerror(int errnum)
{
    static char s_err_storage[256];

    const char *text = resolve_err_text(errnum);

    if (text) {
        util_strcpy(s_err_storage, text, sizeof(s_err_storage));
    } else {
        format_unknown(errnum, s_err_storage, sizeof(s_err_storage));
    }

    return s_err_storage;
}
