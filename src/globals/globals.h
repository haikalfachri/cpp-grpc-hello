
#include <vector>
#include "../database/database.h"
#include "../zeromq/zeromq_handler.h"
#include "../sse/sse_handler.h"

extern std::vector<std::string> message_queue;

extern Database* database;
extern Publisher* publisher;
extern Subscriber* subscriber;
extern SSEServer* sse_server;



