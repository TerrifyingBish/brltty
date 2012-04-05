/*
 * BRLTTY - A background process providing access to the console screen (when in
 *          text mode) for a blind person using a refreshable braille display.
 *
 * Copyright (C) 1995-2012 by The BRLTTY Developers.
 *
 * BRLTTY comes with ABSOLUTELY NO WARRANTY.
 *
 * This is free software, placed under the terms of the
 * GNU General Public License, as published by the Free Software
 * Foundation; either version 2 of the License, or (at your option) any
 * later version. Please see the file LICENSE-GPL for details.
 *
 * Web Page: http://mielke.cc/brltty/
 *
 * This software is maintained by Dave Mielke <dave@mielke.cc>.
 */

#include "prologue.h"

#include <stdio.h>
#include <string.h>
#include <errno.h>

#if defined(__MINGW32__)
#include <ws2tcpip.h>

#elif defined(__MSDOS__)

#elif defined(GRUB_RUNTIME)
typedef unsigned int uid_t;
typedef unsigned int gid_t;

#else /* Unix */
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#include <pwd.h>
#include <grp.h>
#endif /* platform-specific includes */

#if !defined(AF_LOCAL) && defined(AF_UNIX)
#define AF_LOCAL AF_UNIX
#endif /* !defined(AF_LOCAL) && defined(AF_UNIX) */

#if !defined(PF_LOCAL) && defined(PF_UNIX)
#define PF_LOCAL PF_UNIX
#endif /* !defined(PF_LOCAL) && defined(PF_UNIX) */

#include "log.h"
#include "parse.h"
#include "auth.h"

/* peer credentials */
#undef CAN_CHECK_CREDENTIALS

typedef struct {
#ifdef __MINGW32__
  const char *name;
#else /* __MINGW32__ */
  uid_t id;
#endif /* __MINGW32__ */
} MethodDescriptor_user;

typedef struct {
#ifdef __MINGW32__
  const char *name;
#else /* __MINGW32__ */
  gid_t id;
#endif /* __MINGW32__ */
} MethodDescriptor_group;

#if defined(__MINGW32__)
#define CAN_CHECK_CREDENTIALS

typedef struct {
  char *user;
} PeerCredentials;

static int
retrievePeerCredentials (PeerCredentials *credentials, FileDescriptor fd) {
  char buffer[0X100+1];

  if (GetNamedPipeHandleState(fd, NULL, NULL, NULL, NULL, buffer, sizeof(buffer))) {
    buffer[sizeof(buffer) - 1] = 0;

    if ((credentials->user = strdup(buffer))) {
      return 1;
    }
  } else {
    switch (GetLastError()) {
      default:
        logWindowsSystemError("GetNamedPipeHandleState");

      case ERROR_INSUFFICIENT_BUFFER: /* buffer too small */
      case ERROR_INVALID_HANDLE: /* not a named pipe */
      case ERROR_CANNOT_IMPERSONATE: /* no data transferred yet */
        break;
    }
  }

  return 0;
}

static void
releasePeerCredentials (PeerCredentials *credentials) {
  free(credentials->user);
}

static int
checkPeerUser (PeerCredentials *credentials, const MethodDescriptor_user *user) {
  return strcmp(user->name, credentials->user) == 0;
}

static int
checkPeerGroup (PeerCredentials *credentials, const MethodDescriptor_group *group) {
  return 0;
}

#elif defined(HAVE_GETPEERUCRED)
#define CAN_CHECK_CREDENTIALS

#include <ucred.h>

#ifdef HAVE_GETZONEID
#include <zone.h>
#endif /* HAVE_GETZONEID */

typedef ucred_t *PeerCredentials;

static int
retrievePeerCredentials (PeerCredentials *credentials, int fd) {
  *credentials = NULL;
  if (getpeerucred(fd, credentials) == -1) {
    logSystemError("getpeerucred");
    return 0;
  }

#ifdef HAVE_GETZONEID
  if (ucred_getzoneid(*credentials) != getzoneid()) {
    ucred_free(*credentials);
    return 0;
  }
#endif /* HAVE_GETZONEID */

  return 1;
}

static void
releasePeerCredentials (PeerCredentials *credentials) {
  ucred_free(*credentials);
}

static int
checkPeerUser (PeerCredentials *credentials, const MethodDescriptor_user *user) {
  if (user->id == ucred_geteuid(*credentials)) return 1;
  return 0;
}

static int
checkPeerGroup (PeerCredentials *credentials, const MethodDescriptor_group *group) {
  if (group->id == ucred_getegid(*credentials)) return 1;

  {
    const gid_t *groups;
    int count = ucred_getgroups(*credentials, &groups);
    while (count > 0)
      if (group->id == groups[--count])
        return 1;
  }

  return 0;
}

#elif defined(SO_PEERCRED)
#define CAN_CHECK_CREDENTIALS

typedef struct ucred PeerCredentials;

static int
retrievePeerCredentials (PeerCredentials *credentials, int fd) {
  socklen_t length = sizeof(*credentials);
  if (getsockopt(fd, SOL_SOCKET, SO_PEERCRED, credentials, &length) != -1) return 1;
  logSystemError("getsockopt[SO_PEERCRED]");
  return 0;
}

static void
releasePeerCredentials (PeerCredentials *credentials) {
}

static int
checkPeerUser (PeerCredentials *credentials, const MethodDescriptor_user *user) {
  return user->id == credentials->uid;
}

static int
checkPeerGroup (PeerCredentials *credentials, const MethodDescriptor_group *group) {
  return group->id == credentials->gid;
}

#elif defined(HAVE_GETPEEREID)
#define CAN_CHECK_CREDENTIALS

typedef struct {
  uid_t euid;
  gid_t egid;
} PeerCredentials;

static int
retrievePeerCredentials (PeerCredentials *credentials, int fd) {
  if (getpeereid(fd, &credentials->euid, &credentials->egid) != -1) return 1;
  logSystemError("getpeereid");
  return 0;
}

static void
releasePeerCredentials (PeerCredentials *credentials) {
}

static int
checkPeerUser (PeerCredentials *credentials, const MethodDescriptor_user *user) {
  return user->id == credentials->euid;
}

static int
checkPeerGroup (PeerCredentials *credentials, const MethodDescriptor_group *group) {
  return group->id == credentials->egid;
}

#else /* peer credentials method */
#warning peer credentials support not available on this platform
#endif /* peer credentials method */

/* general type definitions */

typedef int (*MethodPerform) (AuthDescriptor *auth, FileDescriptor fd, void *data);

typedef struct {
  const char *name;
  void *(*initialize) (const char *parameter);
  void (*release) (void *data);
  MethodPerform client;
  MethodPerform server;
} MethodDefinition;

typedef struct {
  const MethodDefinition *definition;
  void *data;
} MethodDescriptor;

#ifdef CAN_CHECK_CREDENTIALS
typedef enum {
  PCS_NEED,
  PCS_CANT,
  PCS_HAVE
} PeerCredentialsState;
#endif /* CAN_CHECK_CREDENTIALS */

typedef int (*AuthPerform) (AuthDescriptor *auth, FileDescriptor fd);

struct AuthDescriptorStruct {
  int count;
  char **parameters;
  MethodDescriptor *methods;
  AuthPerform perform;

#ifdef CAN_CHECK_CREDENTIALS
  PeerCredentialsState peerCredentialsState;
  PeerCredentials peerCredentials;
#endif /* CAN_CHECK_CREDENTIALS */
};

/* the keyfile method */

typedef struct {
  const char *path;
} MethodDescriptor_keyfile;

static void *
authKeyfile_initialize (const char *parameter) {
  MethodDescriptor_keyfile *keyfile;

  if ((keyfile = malloc(sizeof(*keyfile)))) {
    if (*parameter) {
      keyfile->path = parameter;
      return keyfile;
    } else {
      logMessage(LOG_ERR, "path to key file not specified");
    }

    free(keyfile);
  } else {
    logMallocError();
  }

  return NULL;
}

static void
authKeyfile_release (void *data) {
  MethodDescriptor_keyfile *keyfile = data;
  free(keyfile);
}

static int
authKeyfile_client (AuthDescriptor *auth, FileDescriptor fd, void *data) {
  return 1;
}

static int
authKeyfile_server (AuthDescriptor *auth, FileDescriptor fd, void *data) {
  MethodDescriptor_keyfile *keyfile = data;
  logMessage(LOG_DEBUG, "checking key file: %s", keyfile->path);
  return 1;
}

#ifdef CAN_CHECK_CREDENTIALS
static int
getPeerCredentials (AuthDescriptor *auth, FileDescriptor fd) {
  if (auth->peerCredentialsState == PCS_NEED) {
    auth->peerCredentialsState = retrievePeerCredentials(&auth->peerCredentials, fd)? PCS_HAVE: PCS_CANT;
  }
  return auth->peerCredentialsState == PCS_HAVE;
}

/* the user method */

static void *
authUser_initialize (const char *parameter) {
  MethodDescriptor_user *user;

  if ((user = malloc(sizeof(*user)))) {
#ifdef __MINGW32__
    user->name = parameter;
    return user;
#else /* __MINGW32__ */
    if (!*parameter) {
      user->id = geteuid();
      return user;
    }

    {
      int value;
      if (isInteger(&value, parameter)) {
        user->id = value;
        return user;
      }
    }

    {
      const struct passwd *p = getpwnam(parameter);
      if (p) {
        user->id = p->pw_uid;
        return user;
      }
    }

    logMessage(LOG_ERR, "unknown user: %s", parameter);
    free(user);
#endif /* __MINGW32__ */
  } else {
    logMallocError();
  }

  return NULL;
}

static void
authUser_release (void *data) {
  MethodDescriptor_user *user = data;
  free(user);
}

static int
authUser_server (AuthDescriptor *auth, FileDescriptor fd, void *data) {
  MethodDescriptor_user *user = data;
  return getPeerCredentials(auth, fd) &&
         checkPeerUser(&auth->peerCredentials, user);
}

/* the group method */

static void *
authGroup_initialize (const char *parameter) {
  MethodDescriptor_group *group;

  if ((group = malloc(sizeof(*group)))) {
#ifdef __MINGW32__
    group->name = parameter;
    return group;
#else /* __MINGW32__ */
    if (!*parameter) {
      group->id = getegid();
      return group;
    }

    {
      int value;
      if (isInteger(&value, parameter)) {
        group->id = value;
        return group;
      }
    }

    {
      const struct group *g = getgrnam(parameter);
      if (g) {
        group->id = g->gr_gid;
        return group;
      }
    }

    logMessage(LOG_ERR, "unknown group: %s", parameter);
    free(group);
#endif /* __MINGW32__ */
  } else {
    logMallocError();
  }

  return NULL;
}

static void
authGroup_release (void *data) {
  MethodDescriptor_group *group = data;
  free(group);
}

static int
authGroup_server (AuthDescriptor *auth, FileDescriptor fd, void *data) {
  MethodDescriptor_group *group = data;
  return getPeerCredentials(auth, fd) &&
         checkPeerGroup(&auth->peerCredentials, group);
}
#endif /* CAN_CHECK_CREDENTIALS */

/* general functions */

static const MethodDefinition methodDefinitions[] = {
  { .name = "keyfile",
    .initialize = authKeyfile_initialize,
    .release = authKeyfile_release,
    .client = authKeyfile_client,
    .server = authKeyfile_server
  },

#ifdef CAN_CHECK_CREDENTIALS
  { .name = "user",
    .initialize = authUser_initialize,
    .release = authUser_release,
    .client = NULL,
    .server = authUser_server
  },

  { .name = "group",
    .initialize = authGroup_initialize,
    .release = authGroup_release,
    .client = NULL,
    .server = authGroup_server
  },
#endif /* CAN_CHECK_CREDENTIALS */

  {.name = NULL}
};

static void
releaseMethodDescriptor (MethodDescriptor *method) {
  if (method->data) {
    if (method->definition->release) method->definition->release(method->data);
    method->data = NULL;
  }
}

static void
releaseMethodDescriptors (AuthDescriptor *auth, int count) {
  while (count > 0) releaseMethodDescriptor(&auth->methods[--count]);
}

static int
initializeMethodDescriptor (MethodDescriptor *method, const char *parameter) {
  const char *name;
  int nameLength;

  if ((parameter = strchr(name=parameter, ':'))) {
    nameLength = parameter++ - name;
  } else {
    parameter = name + (nameLength = strlen(name));
  }

  {
    const MethodDefinition *definition = methodDefinitions;
    while (definition->name) {
      if ((nameLength == strlen(definition->name)) &&
          (strncmp(name, definition->name, nameLength) == 0)) {
        void *data = definition->initialize(parameter);
        if (!data) return 0;

        method->definition = definition;
        method->data = data;
        return 1;
      }

      ++definition;
    }
  }

  logMessage(LOG_WARNING, "unknown authentication/authorization method: %.*s", nameLength, name);
  return 0;
}

static int
initializeMethodDescriptors (AuthDescriptor *auth) {
  int index = 0;
  while (index < auth->count) {
    if (!initializeMethodDescriptor(&auth->methods[index], auth->parameters[index])) {
      releaseMethodDescriptors(auth, index);
      return 0;
    }
    ++index;
  }
  return 1;
}

static AuthDescriptor *
authBegin (const char *parameter, const char *defaultParameter, AuthPerform perform) {
  AuthDescriptor *auth;

  if ((auth = malloc(sizeof(*auth)))) {
    auth->perform = perform;

    if (!parameter) parameter = "";
    if (!*parameter) {
      parameter = defaultParameter;
    } else if (strcmp(parameter, "none") == 0) {
      parameter = "";
    }

    if ((auth->parameters = splitString(parameter, '+', &auth->count))) {
      if (!auth->count) {
        auth->methods = NULL;
        return auth;
      }

      if ((auth->methods = malloc(ARRAY_SIZE(auth->methods, auth->count)))) {
        if (initializeMethodDescriptors(auth)) return auth;

        free(auth->methods);
      } else {
        logMallocError();
      }

      deallocateStrings(auth->parameters);
    }

    free(auth);
  } else {
    logMallocError();
  }

  return NULL;
}

static int
authPerformClient (AuthDescriptor *auth, FileDescriptor fd) {
  return 1;
}

AuthDescriptor *
authBeginClient (const char *parameter) {
  return authBegin(parameter, "", authPerformClient);
}

static int
authPerformServer (AuthDescriptor *auth, FileDescriptor fd) {
  int ok = 0;

  if (!auth->count) return 1;

#ifdef CAN_CHECK_CREDENTIALS
  auth->peerCredentialsState = PCS_NEED;
#endif /* CAN_CHECK_CREDENTIALS */

  {
    int index;
    for (index=0; index<auth->count; ++index) {
      const MethodDescriptor *method = &auth->methods[index];

      if (!method->definition->server) continue;
      if (method->definition->client) continue;

      if (method->definition->server(auth, fd, method->data)) {
        ok = 1;
        break;
      }
    }
  }

#ifdef CAN_CHECK_CREDENTIALS
  if (auth->peerCredentialsState != PCS_NEED) {
    if (auth->peerCredentialsState == PCS_HAVE) releasePeerCredentials(&auth->peerCredentials);
    if (!ok) logMessage(LOG_ERR, "no matching user or group");
  }
#endif /* CAN_CHECK_CREDENTIALS */

  if (!ok) {
    int index;
    for (index=0; index<auth->count; ++index) {
      const MethodDescriptor *method = &auth->methods[index];

      if (!method->definition->server) continue;
      if (!method->definition->client) continue;
    }
  }

  return ok;
}

AuthDescriptor *
authBeginServer (const char *parameter) {
  return authBegin(parameter, "user", authPerformServer);
}

void
authEnd (AuthDescriptor *auth) {
  releaseMethodDescriptors(auth, auth->count);
  if (auth->methods) free(auth->methods);
  deallocateStrings(auth->parameters);
  free(auth);
}

int
authPerform (AuthDescriptor *auth, FileDescriptor fd) {
  return auth->perform(auth, fd);
}

void
formatAddress (char *buffer, int bufferSize, const void *address, int addressSize) {
#ifdef AF_INET
  const struct sockaddr *sa = address;
  switch (sa->sa_family) {
#ifndef __MINGW32__
    case AF_LOCAL: {
      const struct sockaddr_un *local = address;
      if (addressSize <= sizeof(sa_family_t)) {
        snprintf(buffer, bufferSize, "local <unnamed>");
      } else {
        snprintf(buffer, bufferSize, "local %s", local->sun_path);
      }
      break;
    }
#endif /* __MINGW32__ */

    case AF_INET: {
      const struct sockaddr_in *inet = address;
      snprintf(buffer, bufferSize, "inet %s:%d", inet_ntoa(inet->sin_addr), ntohs(inet->sin_port));
      break;
    }

    default:
#if defined(HAVE_GETNAMEINFO) && !defined(WINDOWS)
      {
        char host[NI_MAXHOST];
        char service[NI_MAXSERV];
        int err;

        if (!(err = getnameinfo(address, addressSize,
                                host, sizeof(host), service, sizeof(service),
                                NI_NUMERICHOST | NI_NUMERICSERV))) {
          snprintf(buffer, bufferSize, "af=%d %s:%s", sa->sa_family, host, service);
          break;
        }

        if (err != EAI_FAMILY) {
#ifdef HAVE_GAI_STRERROR
          snprintf(buffer, bufferSize, "reverse lookup error for address family %d: %s", 
                   sa->sa_family,
#ifdef EAI_SYSTEM
                   (err == EAI_SYSTEM)? strerror(errno):
#endif /* EAI_SYSTEM */
                   gai_strerror(err));
#else /* HAVE_GAI_STRERROR */
          snprintf(buffer, bufferSize, "reverse lookup error %d for address family %d.",
                   err, sa->sa_family);
#endif /* HAVE_GAI_STRERROR */
          break;
        }
      }
#endif /* GETNAMEINFO */

      {
        int length;
        snprintf(buffer, bufferSize, "address family %d:%n", sa->sa_family, &length);

        {
          const unsigned char *byte = address;
          const unsigned char *end = byte + addressSize;
          while (byte < end) {
            int count;
            snprintf(&buffer[length], bufferSize-length,
                     " %02X%n", *byte++, &count);
            length += count;
          }
        }
      }
      break;
  }
#else /* AF_INET */
  snprintf(buffer, bufferSize, "unknown");
#endif /* AF_INET */
}
