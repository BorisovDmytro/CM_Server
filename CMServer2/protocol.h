#ifndef PROTOCOL_H
#define PROTOCOL_H

enum MessageType {
  Undefined = -1, // Not defined type
  CallFrame = 0, // voice data chank
  StartCall,  // send start call
  SuccessCall,
  CanselCall,
  EndCall,
  TextMessage,
  Auth,
  GetUserList
};

#endif // PROTOCOL_H
