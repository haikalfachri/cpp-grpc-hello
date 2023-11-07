#pragma once

#include <vector>
#include "../database/database.h"
#include "../sse/sse_handler.h"
#include "../zeromq/zeromq_handler.h"
#include "../env/env_reader.h"

extern Database* database;
extern Publisher* publisher;
extern Subscriber* subscriber;
extern SSEServer* sse_server;

extern EnvReader* env_reader;