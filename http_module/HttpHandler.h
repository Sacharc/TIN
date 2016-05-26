//
// Created by michto on 25.05.16.
//

#ifndef TIN_HTTPHANDLER_H
#define TIN_HTTPHANDLER_H

#include "../manager/DetectorHistory.h"
#include "../manager/MessageHandler.h"

void httpHandlerStart(MessageHandler*);
const char* buildHtmlString(std::vector<HistoryRecord*>*);

#endif //TIN_HTTPHANDLER_H
