/**
 * @file http_code.h
 * @author hbuyse
 * @date 10/05/2016
 *
 * @brief  HTTP status code available
 */

#ifndef __HTTP_CODE_H__
#define __HTTP_CODE_H__


#ifdef __cplusplus
extern "C" {
#endif


// 1xx Informational
#define HTTP_CONTINUE 100          ///< Continue
#define HTTP_SWITCHING_PROTOCOLS 101          ///< Switching protocols
#define HTTP_PROCESSING 102          ///< Processing


// 2xx Success
#define HTTP_OK 200          ///< Ok
#define HTTP_CREATED 201          ///< Created
#define HTTP_ACCEPTED 202          ///< Accepted
#define HTTP_NON_AUTHORITATIVE_INFORMATION 203          ///< Non authoritative information
#define HTTP_NO_CONTENT 204          ///< No content
#define HTTP_RESET_CONTENT 205          ///< Reset content
#define HTTP_PARTIAL_CONTENT 206          ///< Partial content
#define HTTP_MULTI_STATUS 207          ///< Multi status
#define HTTP_ALREADY_REPORTED 208          ///< Already reported
#define HTTP_IM_USED 226          ///< I'm used


// 3xx Redirection
#define HTTP_MULTIPLE_CHOICES 300          ///< Multiple choices
#define HTTP_MOVED_PERMANENTLY 301          ///< Moved permanently
#define HTTP_FOUND 302          ///< Found
#define HTTP_SEE_OTHER 303          ///< See other
#define HTTP_NOT_MODIFIED 304          ///< Not modified
#define HTTP_USE_PROXY 305          ///< Use proxy
#define HTTP_SWITCH_PROXY 306          ///< Switch proxy
#define HTTP_TEMPORARY_REDIRECT 307          ///< Temporary redirect
#define HTTP_PERMANENT_REDIRECT 308          ///< Permanent redirect


// 4xx Error
#define HTTP_BAD_REQUEST 400            ///< Bad request
#define HTTP_UNAUTHORIZED 401           ///< Unauthorized
#define HTTP_PAYMENT_REQUIRED 402           ///< Payment required
#define HTTP_FORBIDDEN 403          ///< Forbidden
#define HTTP_NOT_FOUND 404          ///< Not found
#define HTTP_METHOD_NOT_ALLOWED 405          ///< Method not allowed
#define HTTP_NOT_ACCEPTABLE 406          ///< Not acceptable
#define HTTP_PROXY_AUTHENTICATION_REQUIRED 407          ///< Proxy authentication required
#define HTTP_REQUEST_TIMEOUT 408            ///< Request timeout
#define HTTP_CONFLICT 409           ///< Conflict
#define HTTP_GONE 410           ///< Gone
#define HTTP_LENGTH_REQUIRED 411            ///< Length required
#define HTTP_PRECONDITION_FAILED 412            ///< Precondition failed
#define HTTP_PAYLOAD_TOO_LARGE 413          ///< Payload too large
#define HTTP_URI_TOO_LONG 414           ///< Uri too long
#define HTTP_UNSUPPORTED_MEDIA_TYPE 415          ///< Unsupported media type
#define HTTP_RANGE_NOT_SATISFIABLE 416          ///< Range not satisfiable
#define HTTP_EXPECTATION_FAILED 417          ///< Expectation failed
#define HTTP_I_M_A_TEAPOT 418           ///< I'm a teapot
#define HTTP_MISDIRECTED_REQUEST 421            ///< Misdirected request
#define HTTP_UNPROCESSABLE_ENTITY 422           ///< Unprocessable entity
#define HTTP_LOCKED 423          ///< Locked
#define HTTP_FAILED_DEPENDENCY 424          ///< Failed dependency
#define HTTP_UPGRADE_REQUIRED 426           ///< Upgrade required
#define HTTP_PRECONDITION_REQUIRED 428          ///< Precondition required
#define HTTP_TOO_MANY_REQUESTS 429          ///< Too many requests
#define HTTP_REQUEST_HEADER_FIELDS_TOO_LARGE 431            ///< Request header fields too large
#define HTTP_UNAVAILABLE_FOR_LEGAL_REASONS 451          ///< Unavailable for legal reasons


// 5xx Server Error
#define HTTP_INTERNAL_SERVER_ERROR 500          ///< Internal server error
#define HTTP_NOT_IMPLEMENTED 501          ///< Not implemented
#define HTTP_BAD_GATEWAY 502          ///< Bad gateway
#define HTTP_SERVICE_UNAVAILABLE 503          ///< Service unavailable
#define HTTP_GATEWAY_TIMEOUT 504          ///< Gateway timeout
#define HTTP_HTTP_VERSION_NOT_SUPPORTED 505          ///< HTTP version not supported
#define HTTP_VARIANT_ALSO_NEGOTIATES 506          ///< Variant also negotiates
#define HTTP_INSUFFICIENT_STORAGE 507          ///< Insufficient storage
#define HTTP_LOOP_DETECTED 508          ///< Loop detected
#define HTTP_NOT_EXTENDED 510          ///< Not extended
#define HTTP_NETWORK_AUTHENTICATION_REQUIRED 511          ///< Network authentication required


#ifdef __cplusplus
}
#endif

#endif          // __HTTP_CODE_H__