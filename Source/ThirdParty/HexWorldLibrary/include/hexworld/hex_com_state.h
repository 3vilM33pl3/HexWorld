#pragma once

typedef enum {
    /** connection is idle */
    HEXWORLD_CONNECTION_IDLE,
    /** connecting to server */
    HEXWORLD_CONNECTING,
    /** connection to server ready */
    HEXWORLD_CONNECTION_READY,
    /** retrying connection to server */
    HEXWORLD_CONNECTION_RETRY,
    /** connection failure that it can not recover from */
    HEXWORLD_CONNECTION_FATAL,
    /** connection timeout */
    HEXWORLD_CONNECTION_TIMEOUT
} hw_conn_state;
