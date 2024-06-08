// Queue.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <windows.h>
#include <atlbase.h>
//#import "mqoa.dll"
//using namespace MSMQ;

int main()
{
    // Initialize the COM library
    CoInitialize(NULL);

    //// Create a smart pointer to the MSMQQueueInfo interface
    //IMSMQQueueInfoPtr pInfo("MSMQ.MSMQQueueInfo");

    //// Set the format name of the queue
    //pInfo->FormatName = "DIRECT=OS:localhost\\private$\\MyQueue";

    //// Open the queue to send messages
    //IMSMQQueuePtr pQueue = pInfo->Open(MQ_SEND_ACCESS, MQ_DENY_NONE);

    //// Create a smart pointer to the MSMQMessage interface
    //IMSMQMessagePtr pMsg("MSMQ.MSMQMessage");

    //// Set the body of the message
    //pMsg->Body = "Hello, MSMQ!";

    //// Send the message to the queue
    //pMsg->Send(pQueue);

    //// Close the queue
    //pQueue->Close();

    //// Open the queue to receive messages
    //pQueue = pInfo->Open(MQ_RECEIVE_ACCESS, MQ_DENY_NONE);

    //// Receive the message from the queue
    //pMsg = pQueue->Receive();

    //// Print the body of the message
    //printf("Received message: %s\n", (char*)pMsg->Body);

    //// Close the queue
    //pQueue->Close();

    //// Uninitialize the COM library
    //CoUninitialize();

    return 0;
}