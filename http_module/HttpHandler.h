//
// Created by michto on 25.05.16.
//

#ifndef TIN_HTTPHANDLER_H
#define TIN_HTTPHANDLER_H

#include "../manager/DetectorHistory.h"

void httpHandlerStart(std::vector<HistoryRecord*>*);
const char* buildHtmlString(std::vector<HistoryRecord*>* history);

#endif //TIN_HTTPHANDLER_H
