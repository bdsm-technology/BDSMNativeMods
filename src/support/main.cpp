#include <StaticHook.h>
#include <base.h>
#include <functional>
#include <list>
#include <mods.h>
#include <string>

static ServerInstance *si = nullptr;
static std::list<std::function<void(ServerInstance *)>> hookList;

EXPORT void initString(const char *inp, std::string *data);
EXPORT void setString(const char *inp, std::string *data);
EXPORT void deleteString(std::string *str);

EXPORT void regServerHook(void (*fn)(ServerInstance *));
EXPORT void regServerHook2(std::function<void(ServerInstance *)> fn);

EXPORT void queueTask(void (*fn)());
EXPORT void queueTask2(const std::function<void()> &fn);

void initString(const char *inp, std::string *data) { new (data) std::string(inp); }

void setString(const char *inp, std::string *data) { *data = inp; }

void deleteString(std::string *str) { str->~basic_string(); }

void regServerHook(void (*fn)(ServerInstance *)) { hookList.emplace_back(fn); }

void regServerHook2(std::function<void(ServerInstance *)> fn) { hookList.emplace_back(fn); }

void queueTask(void (*fn)()) {
  if (si) { si->queueForServerThread(fn); }
  fn();
}

void queueTask2(const std::function<void()> &fn) {
  if (si) { si->queueForServerThread(fn); }
  fn();
}

TInstanceHook(void, _ZN14ServerInstance17startServerThreadEv, ServerInstance) {
  original(this);
  si = this;
  for (auto &hook : hookList) { hook(this); }
}
