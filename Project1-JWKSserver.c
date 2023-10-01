/*Author: Nupur Rishi (NupurRishi@my.unt.edu)
   EUID: nr0335
   Date: 10/01/2023
   Section: CSCE 3550.001
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>

#define SERVER_PORT 8080

void send_response(int client_socket, int status_code, const char* content) {
    char response[512];
    snprintf(response, sizeof(response),
             "HTTP/1.1 %d Method Not Allowed\r\n"
             "Content-Length: %lu\r\n\r\n%s",
             status_code, (unsigned long)strlen(content), content);

    send(client_socket, response, strlen(response), 0);
}

void GET_request(int client_socket, const char* request) {
    if (strcmp(request, "/well-known/jwks.json") == 0) {
    const char* jwks_data = 
    "{"
    "  \"keys\": ["
    "    {"
    "      \"kid\": \"key-1\","
    "      \"kty\": \"RSA\","
    "      \"use\": \"sig\","
    "      \"n\": \"LS0tLS1CRUdJTiBSU0EgUFJJVkFURSBLRVktLS0tLQpNSUlKS1FJQkFBS0NBZ0VBenhXWDh0UzdwNGtoWFAzYS80b201R0I1ME1DZHg5Qy9TZVFVZGZINmZ6UFpRMExNCkloVWY5VVlZREE5Tk9RamlMOWFvM3NIbWJNM0R5YWUxTi9OQlQvVlMvRGo1bVQ1YlVFQlcyV1hMSVQxalFubEEKRWRuNitjVnNHZ0U5QkFMQnZlbHArWTNZNG9vMlRjcEkwWUo2c3diYjd2Z1kzdlFOd1EzVTVIRmVMTWkxT0FjOAozL3FEQmw4bm9VbUpLTFJrODVkSmVWaUVNZ3E1c0R4c1FpMDhUdUF0NzJ3cnh1NVVQOXVOLzJxM0ZlYVl3bFh4CkNhYjVMOXJ4T3E1RkJ6ZkRtTDFxVFB6U1piRHdScmNWeGoxTkNMeE56d3oxMmg5eEVKSm1ZME9SVnNmb1VkREEKSVFzRUN0UG5DWmgzUy9oMUFTeEd5V1JvVEdyc1FqY0ZZMnFCazNSb2lVRlY2M2lNbFE5MC9vU1N0ZTYrMGZnQgpHMWxIY0E5b2h1QXhvNVo1dG50UlVmU2QvT1BnMS9pZFFSa3Y4Tkt6QUpockhFTmdqcml4aUpEN3dWMHJKNXk5ClVtaDFsRU55b2YvU2FGZ2pBbUpESE5yRDQvZmx0Wmlsd2RGYlF0WjdMRGVrOGVzeE9RTmM3NmdVT2F4VlNuSE0KeTJGWEI2VU9NN1N2dGZzK3hlZ0JZSE15aTE0aFlhLytJNVA0RFcxWTVHY0VVdUNpVDMxbmpXcCtCMXplcEU3Tgo2U0FZYWJOZDVEalVpdzNaY2NwSFI5blF2VWw2UHdoRGRraDY5MU5qVmg2dzEzYzRURHFCM08rQkdYbFNESWtPClZNYklNT21weVpldHN3MTZVclI2SkJzTnVTU2J1c3krcm5DVzdlK2ZkazRlTTZkZ3htS0g3ZzY5elNVQ0F3RUEKQVFLQ0FnQlBUZ3UwMlRkTVA3M2hLTzZYY2tiV3F3TzdxQ2hFZjNMaVQxQnVvczYxckJ1R0lLeWdXTXEyV2JPWAoycnhPSFZMUjB6azBwYmtpN2VJTzlnVDJlbGdHMFFLSXlTRDlTTjRiYTZoVzlSN0hpZ1RiVGlmMUxpOXBPK3ZjClFhRW1TZy84Tk1sNjRwR3VvQk9QMHVuMElpM3ZKYTlRWXY3VFUwOTVJVnpvWjJvVGFJSE9CU2VROXhUNmViSnAKbUFuSTV0ektWS08wcnV0aFpxNGFDZW5EcGlNMGFiSHR3TlU3QjhhSDlnTDVUOXJ4cmkvT0xLOTBiTmQ5TmpHdgpzVkdLSkJicUJaQVNEN2hiYlZsM0cySmhXdFNIaGVXZVoyNDNyRmJXYTlwK0pVQWhwY2tZbkFNWUpHblY0N1RVCi9laWgxWEtwa2xWQmh3eFNSNWhXKzA4NjlXek5aTUFvbXNhWjJMaUs2V0tNakh1YTYyMGQyNC9LQmdzYVBNUUoKNjdvK09EMkJNSWRrQ0hmaC8vYjRzRnJXOVgzZS9kSjcxakdTSUNkUzM5TnFEeklBRDB5NDBwT0EyWE1WTml4ZQpxVmM5WFFiaVpTR3E3dzk2QWVqaTEwVUZFcUIyMjhLQnFReVNaQi9RYk1JUGl1REpIMXA3ZTMrTEk1aWdWWDNlCnNFQ3RmWWFFQmw0MllpQkZJb2Y5cVB0SVdFcmtPKzNVQTJQK0pTczFSVzlLVUN0TlA1WDIvZkJDalB4MnpySkwKbFgvbHhnTVNySUh6ajBjS25SeEV2MXVVRk1IT0QwcDdFRGw4eW9rYlVkYUFZTmlycTU5SEkwYVo4WE9zMndiTAp6Y0JTMWdYRXprenhiNTUyUUxSbytEZHVlRytxZ0ZMWlRMSGMvaUJEQ0tIQkRnZ2hPUUtDQVFFQTlxQlZmNzJ6Cm5IYWlCeVl0OWVaakQ0US9sUTFoQ3dDbGVTdDNTd1djWGhJczhickVlSkF4a1FMSEhGTlE5elNGOGZmUXRoWHYKSU1VT0J6QlhjWlZqOU95b3ovdGJiek9qdGVHd1MxdUtvOHVmS0V3NWxxVlRBcE9EQzJDR2h5QnhmeFRNZ3N0TAp3R1BGbmRrNVF2aEJ4UkV4ZWVqaG9LY2E5ekJXOHN2YU85eWNxdUFyVDZWS3BnRUpldVJpd0M2L0ZMaktCRFl1CmU1eENyK0Y2Mzk4M0tpeGZBcUxONW45NjhwczBJZC9pMFFEcWVuc3B0Y1ZBS24wNmFGeHVSL3Y5MXp2VWlVRWUKOExuVFZBbmhRS2h4cVB1L3ZXVGgzRnN0bUxDTFhvMmU5V1JGd3RBMGZZY1dhQm9QM1FUWjhmaERpYllqbjNTRgpkaWNMSDNKbFI1djhsd0tDQVFFQTF2U0VncXNwMmcvcUtML2FHQkRYUEwzL0FvRWFyVjYzdlJQT0ZLRTAzdVJVCmZ6MUd3bFUxd0pHa0EwY3c1a05DOEE2VityZUFUeFhLV0o1bjZUSkZVbWg2TWxONzZUUVV6N3BPa1o5QU8rQnIKcHNjTWpGOURwZjhrS3hEc0pEVjFNVEoyTVJxN0hHWGxCTlZ6ekxlNC9QNytmVUZXdDlES0tUNTBZN3JnbWNrLwpQY3p0YUs1SnVsYWx3aWcrZnU0VStSSXVQaitTTWphNFVkcGVNTGlxZ3BkR2l2emF2NGlkWFlzODJodVJSS2FBCkxReTVWT3JQRzNNODJkWFJ3VHlWb3RQSDVUeFgyWEVNN3dTam4ybkdyUk9BcFpQazNxaXVEcktNYVMrOVhybXIKdDN4SWpjSDdEbnpMb1p3dWlxekFvbDBEOFZ2ck1FRUFvaWtmRzUzdm93S0NBUUFGbVg1WGVDY3Ixa1czZzlabgpUdVBRNWwvTE1wS1JmV0pFYmdnM1c5bEJDWWlidEhIaGl1U1ZOa3J2Q2JVODU2Umk2ZzJERk54cEVoQlBlWG1DCnlyR0tUaFZISTUzZjJIeFBGT0M3MVVoQWo4NzlTejNEZmQxMG9ZVElMVmpnbUl2N0pxMitDbitkenFZZC8yQ2QKZEtXSm8rcExxazMxSjlZazVxUVN2eTRsVGMwQTFEWW5pTWs1RzZ6cmJXUVFtUDhkcVZSczVBNEtJSElqTEN0dQpDZE5tMGJzV2hEdklqL3hxcGJ0NVNXd004c2hnSDR2QzVNZWdMMWUwSnVVYUhVNldKSFdlNkN1YnE1eHd2QWl4ClB5SFZoR0hHR3hSRnZJdzhlMFd1eitoY0NmckxnWTZ2aXZHblMzQXd4SHR1VXdTMWk3MGpVYkgyRE16ajJlVVMKeXlmaEFvSUJBUUNYb0hIY2pvcCtKSjNEVXRtd0Fpa2gwdEFRWHl1SFFmb1A2T21FaWRaMnA4K20xWDJWVlhBTwpQMjNYVmg5N1BkVDQ2d0VHa3V4eVhPZVlMVjBWRlAzclRJRnVBSSsyaDZOcWlId2xvcFUzNTIxb2ZwNHVZb0NnCmxtSlg4cDd3UjdrOEUrRmxiL3F1NUgxeE5IUno0dUVkclcyc3d5czlqSTBlSE0wVEVGNmdmR0ljNFBKUzFpWngKeCs4ekR4cE9oUGpNWFBPN3V6SVFNNk0vTWU4aEpSSTUvaVdub3dkVzlqSXpLeVdvb0t1UEpuZmh6MGpvU3AzLwpPZ0doazM5YmRMWUUzcWp4M2VnR0hRWEhrSmt1dVBtSld1bHpZdWlkMjVTMEQyU1dTak9sbndrdTYvOXQ1M2lGCjFxZjVmdWNoc1F4Tk81bXRabTUzWWxMbEVHYStIMkZMQW9JQkFRQ3ZzVmUvN0xFV09LZzFwaFNxY25LM3lub1gKYnFJL3dQd3RRazJLRXpDWlFLb1dSKzJxZTNwb2JuQ2t3a2wwWkJLVkVRaEp2QnJLdGkrdlJKTi9TM1p5QmpvOApyZFlGRHllbHk4Qm5TelpNK0p1V2J0UFMwem82dEtTSi82YmdHSHR1U2pvR0VtS1M0Mm54RGh4SkFpZUkvUm5UCjQ2YU1Oems4NHR0YWc2RzZHaWlTTkt1ZDlST2JuK2lNQm1HYWNscXJPOXBKc3dSMEVnRTNmNlhmSnFvbk5HdjkKZW1MR3NEbS9JUHBOTTV5ZDM2cXNSUnhPWkJ4UmtMOHF5OU4wbGV2Tjd4bDY2WTVOU1JFU0JnUlJNNXBNdmRyMgp1VjFFUGlQZm9PUjJrbEl2b2gza3NDSC9uTU5UT0N4SUg0KzZySU4xWEdCQnhZOG1YRTZWVUV4YUtETlcKLS0tLS1FTkQgUlNBIFBSSVZBVEUgS0VZLS0tLS0K\","
    "      \"e\": \"LS0tLS1CRUdJTiBQVUJMSUMgS0VZLS0tLS0KTUlJQ0lqQU5CZ2txaGtpRzl3MEJBUUVGQUFPQ0FnOEFNSUlDQ2dLQ0FnRUF6eFdYOHRTN3A0a2hYUDNhLzRvbQo1R0I1ME1DZHg5Qy9TZVFVZGZINmZ6UFpRMExNSWhVZjlVWVlEQTlOT1FqaUw5YW8zc0htYk0zRHlhZTFOL05CClQvVlMvRGo1bVQ1YlVFQlcyV1hMSVQxalFubEFFZG42K2NWc0dnRTlCQUxCdmVscCtZM1k0b28yVGNwSTBZSjYKc3diYjd2Z1kzdlFOd1EzVTVIRmVMTWkxT0FjODMvcURCbDhub1VtSktMUms4NWRKZVZpRU1ncTVzRHhzUWkwOApUdUF0NzJ3cnh1NVVQOXVOLzJxM0ZlYVl3bFh4Q2FiNUw5cnhPcTVGQnpmRG1MMXFUUHpTWmJEd1JyY1Z4ajFOCkNMeE56d3oxMmg5eEVKSm1ZME9SVnNmb1VkREFJUXNFQ3RQbkNaaDNTL2gxQVN4R3lXUm9UR3JzUWpjRlkycUIKazNSb2lVRlY2M2lNbFE5MC9vU1N0ZTYrMGZnQkcxbEhjQTlvaHVBeG81WjV0bnRSVWZTZC9PUGcxL2lkUVJrdgo4Tkt6QUpockhFTmdqcml4aUpEN3dWMHJKNXk5VW1oMWxFTnlvZi9TYUZnakFtSkRITnJENC9mbHRaaWx3ZEZiClF0WjdMRGVrOGVzeE9RTmM3NmdVT2F4VlNuSE15MkZYQjZVT003U3Z0ZnMreGVnQllITXlpMTRoWWEvK0k1UDQKRFcxWTVHY0VVdUNpVDMxbmpXcCtCMXplcEU3TjZTQVlhYk5kNURqVWl3M1pjY3BIUjluUXZVbDZQd2hEZGtoNgo5MU5qVmg2dzEzYzRURHFCM08rQkdYbFNESWtPVk1iSU1PbXB5WmV0c3cxNlVyUjZKQnNOdVNTYnVzeStybkNXCjdlK2ZkazRlTTZkZ3htS0g3ZzY5elNVQ0F3RUFBUT09Ci0tLS0tRU5EIFBVQkxJQyBLRVktLS0tLQo\""
    "    },"
    "    {"
    "      \"kid\": \"key-2\","
    "      \"kty\": \"RSA\","
    "      \"use\": \"sig\","
    "      \"n\": \"LS0tLS1CRUdJTiBSU0EgUFJJVkFURSBLRVktLS0tLQpNSUlKS1FJQkFBS0NBZ0VBenhXWDh0UzdwNGtoWFAzYS80b201R0I1ME1DZHg5Qy9TZVFVZGZINmZ6UFpRMExNCkloVWY5VVlZREE5Tk9RamlMOWFvM3NIbWJNM0R5YWUxTi9OQlQvVlMvRGo1bVQ1YlVFQlcyV1hMSVQxalFubEEKRWRuNitjVnNHZ0U5QkFMQnZlbHArWTNZNG9vMlRjcEkwWUo2c3diYjd2Z1kzdlFOd1EzVTVIRmVMTWkxT0FjOAozL3FEQmw4bm9VbUpLTFJrODVkSmVWaUVNZ3E1c0R4c1FpMDhUdUF0NzJ3cnh1NVVQOXVOLzJxM0ZlYVl3bFh4CkNhYjVMOXJ4T3E1RkJ6ZkRtTDFxVFB6U1piRHdScmNWeGoxTkNMeE56d3oxMmg5eEVKSm1ZME9SVnNmb1VkREEKSVFzRUN0UG5DWmgzUy9oMUFTeEd5V1JvVEdyc1FqY0ZZMnFCazNSb2lVRlY2M2lNbFE5MC9vU1N0ZTYrMGZnQgpHMWxIY0E5b2h1QXhvNVo1dG50UlVmU2QvT1BnMS9pZFFSa3Y4Tkt6QUpockhFTmdqcml4aUpEN3dWMHJKNXk5ClVtaDFsRU55b2YvU2FGZ2pBbUpESE5yRDQvZmx0Wmlsd2RGYlF0WjdMRGVrOGVzeE9RTmM3NmdVT2F4VlNuSE0KeTJGWEI2VU9NN1N2dGZzK3hlZ0JZSE15aTE0aFlhLytJNVA0RFcxWTVHY0VVdUNpVDMxbmpXcCtCMXplcEU3Tgo2U0FZYWJOZDVEalVpdzNaY2NwSFI5blF2VWw2UHdoRGRraDY5MU5qVmg2dzEzYzRURHFCM08rQkdYbFNESWtPClZNYklNT21weVpldHN3MTZVclI2SkJzTnVTU2J1c3krcm5DVzdlK2ZkazRlTTZkZ3htS0g3ZzY5elNVQ0F3RUEKQVFLQ0FnQlBUZ3UwMlRkTVA3M2hLTzZYY2tiV3F3TzdxQ2hFZjNMaVQxQnVvczYxckJ1R0lLeWdXTXEyV2JPWAoycnhPSFZMUjB6azBwYmtpN2VJTzlnVDJlbGdHMFFLSXlTRDlTTjRiYTZoVzlSN0hpZ1RiVGlmMUxpOXBPK3ZjClFhRW1TZy84Tk1sNjRwR3VvQk9QMHVuMElpM3ZKYTlRWXY3VFUwOTVJVnpvWjJvVGFJSE9CU2VROXhUNmViSnAKbUFuSTV0ektWS08wcnV0aFpxNGFDZW5EcGlNMGFiSHR3TlU3QjhhSDlnTDVUOXJ4cmkvT0xLOTBiTmQ5TmpHdgpzVkdLSkJicUJaQVNEN2hiYlZsM0cySmhXdFNIaGVXZVoyNDNyRmJXYTlwK0pVQWhwY2tZbkFNWUpHblY0N1RVCi9laWgxWEtwa2xWQmh3eFNSNWhXKzA4NjlXek5aTUFvbXNhWjJMaUs2V0tNakh1YTYyMGQyNC9LQmdzYVBNUUoKNjdvK09EMkJNSWRrQ0hmaC8vYjRzRnJXOVgzZS9kSjcxakdTSUNkUzM5TnFEeklBRDB5NDBwT0EyWE1WTml4ZQpxVmM5WFFiaVpTR3E3dzk2QWVqaTEwVUZFcUIyMjhLQnFReVNaQi9RYk1JUGl1REpIMXA3ZTMrTEk1aWdWWDNlCnNFQ3RmWWFFQmw0MllpQkZJb2Y5cVB0SVdFcmtPKzNVQTJQK0pTczFSVzlLVUN0TlA1WDIvZkJDalB4MnpySkwKbFgvbHhnTVNySUh6ajBjS25SeEV2MXVVRk1IT0QwcDdFRGw4eW9rYlVkYUFZTmlycTU5SEkwYVo4WE9zMndiTAp6Y0JTMWdYRXprenhiNTUyUUxSbytEZHVlRytxZ0ZMWlRMSGMvaUJEQ0tIQkRnZ2hPUUtDQVFFQTlxQlZmNzJ6Cm5IYWlCeVl0OWVaakQ0US9sUTFoQ3dDbGVTdDNTd1djWGhJczhickVlSkF4a1FMSEhGTlE5elNGOGZmUXRoWHYKSU1VT0J6QlhjWlZqOU95b3ovdGJiek9qdGVHd1MxdUtvOHVmS0V3NWxxVlRBcE9EQzJDR2h5QnhmeFRNZ3N0TAp3R1BGbmRrNVF2aEJ4UkV4ZWVqaG9LY2E5ekJXOHN2YU85eWNxdUFyVDZWS3BnRUpldVJpd0M2L0ZMaktCRFl1CmU1eENyK0Y2Mzk4M0tpeGZBcUxONW45NjhwczBJZC9pMFFEcWVuc3B0Y1ZBS24wNmFGeHVSL3Y5MXp2VWlVRWUKOExuVFZBbmhRS2h4cVB1L3ZXVGgzRnN0bUxDTFhvMmU5V1JGd3RBMGZZY1dhQm9QM1FUWjhmaERpYllqbjNTRgpkaWNMSDNKbFI1djhsd0tDQVFFQTF2U0VncXNwMmcvcUtML2FHQkRYUEwzL0FvRWFyVjYzdlJQT0ZLRTAzdVJVCmZ6MUd3bFUxd0pHa0EwY3c1a05DOEE2VityZUFUeFhLV0o1bjZUSkZVbWg2TWxONzZUUVV6N3BPa1o5QU8rQnIKcHNjTWpGOURwZjhrS3hEc0pEVjFNVEoyTVJxN0hHWGxCTlZ6ekxlNC9QNytmVUZXdDlES0tUNTBZN3JnbWNrLwpQY3p0YUs1SnVsYWx3aWcrZnU0VStSSXVQaitTTWphNFVkcGVNTGlxZ3BkR2l2emF2NGlkWFlzODJodVJSS2FBCkxReTVWT3JQRzNNODJkWFJ3VHlWb3RQSDVUeFgyWEVNN3dTam4ybkdyUk9BcFpQazNxaXVEcktNYVMrOVhybXIKdDN4SWpjSDdEbnpMb1p3dWlxekFvbDBEOFZ2ck1FRUFvaWtmRzUzdm93S0NBUUFGbVg1WGVDY3Ixa1czZzlabgpUdVBRNWwvTE1wS1JmV0pFYmdnM1c5bEJDWWlidEhIaGl1U1ZOa3J2Q2JVODU2Umk2ZzJERk54cEVoQlBlWG1DCnlyR0tUaFZISTUzZjJIeFBGT0M3MVVoQWo4NzlTejNEZmQxMG9ZVElMVmpnbUl2N0pxMitDbitkenFZZC8yQ2QKZEtXSm8rcExxazMxSjlZazVxUVN2eTRsVGMwQTFEWW5pTWs1RzZ6cmJXUVFtUDhkcVZSczVBNEtJSElqTEN0dQpDZE5tMGJzV2hEdklqL3hxcGJ0NVNXd004c2hnSDR2QzVNZWdMMWUwSnVVYUhVNldKSFdlNkN1YnE1eHd2QWl4ClB5SFZoR0hHR3hSRnZJdzhlMFd1eitoY0NmckxnWTZ2aXZHblMzQXd4SHR1VXdTMWk3MGpVYkgyRE16ajJlVVMKeXlmaEFvSUJBUUNYb0hIY2pvcCtKSjNEVXRtd0Fpa2gwdEFRWHl1SFFmb1A2T21FaWRaMnA4K20xWDJWVlhBTwpQMjNYVmg5N1BkVDQ2d0VHa3V4eVhPZVlMVjBWRlAzclRJRnVBSSsyaDZOcWlId2xvcFUzNTIxb2ZwNHVZb0NnCmxtSlg4cDd3UjdrOEUrRmxiL3F1NUgxeE5IUno0dUVkclcyc3d5czlqSTBlSE0wVEVGNmdmR0ljNFBKUzFpWngKeCs4ekR4cE9oUGpNWFBPN3V6SVFNNk0vTWU4aEpSSTUvaVdub3dkVzlqSXpLeVdvb0t1UEpuZmh6MGpvU3AzLwpPZ0doazM5YmRMWUUzcWp4M2VnR0hRWEhrSmt1dVBtSld1bHpZdWlkMjVTMEQyU1dTak9sbndrdTYvOXQ1M2lGCjFxZjVmdWNoc1F4Tk81bXRabTUzWWxMbEVHYStIMkZMQW9JQkFRQ3ZzVmUvN0xFV09LZzFwaFNxY25LM3lub1gKYnFJL3dQd3RRazJLRXpDWlFLb1dSKzJxZTNwb2JuQ2t3a2wwWkJLVkVRaEp2QnJLdGkrdlJKTi9TM1p5QmpvOApyZFlGRHllbHk4Qm5TelpNK0p1V2J0UFMwem82dEtTSi82YmdHSHR1U2pvR0VtS1M0Mm54RGh4SkFpZUkvUm5UCjQ2YU1Oems4NHR0YWc2RzZHaWlTTkt1ZDlST2JuK2lNQm1HYWNscXJPOXBKc3dSMEVnRTNmNlhmSnFvbk5HdjkKZW1MR3NEbS9JUHBOTTV5ZDM2cXNSUnhPWkJ4UmtMOHF5OU4wbGV2Tjd4bDY2WTVOU1JFU0JnUlJNNXBNdmRyMgp1VjFFUGlQZm9PUjJrbEl2b2gza3NDSC9uTU5UT0N4SUg0KzZySU4xWEdCQnhZOG1YRTZWVUV4YUtETlcKLS0tLS1FTkQgUlNBIFBSSVZBVEUgS0VZLS0tLS0K\","
    "      \"e\": \"LS0tLS1CRUdJTiBQVUJMSUMgS0VZLS0tLS0KTUlJQ0lqQU5CZ2txaGtpRzl3MEJBUUVGQUFPQ0FnOEFNSUlDQ2dLQ0FnRUF6eFdYOHRTN3A0a2hYUDNhLzRvbQo1R0I1ME1DZHg5Qy9TZVFVZGZINmZ6UFpRMExNSWhVZjlVWVlEQTlOT1FqaUw5YW8zc0htYk0zRHlhZTFOL05CClQvVlMvRGo1bVQ1YlVFQlcyV1hMSVQxalFubEFFZG42K2NWc0dnRTlCQUxCdmVscCtZM1k0b28yVGNwSTBZSjYKc3diYjd2Z1kzdlFOd1EzVTVIRmVMTWkxT0FjODMvcURCbDhub1VtSktMUms4NWRKZVZpRU1ncTVzRHhzUWkwOApUdUF0NzJ3cnh1NVVQOXVOLzJxM0ZlYVl3bFh4Q2FiNUw5cnhPcTVGQnpmRG1MMXFUUHpTWmJEd1JyY1Z4ajFOCkNMeE56d3oxMmg5eEVKSm1ZME9SVnNmb1VkREFJUXNFQ3RQbkNaaDNTL2gxQVN4R3lXUm9UR3JzUWpjRlkycUIKazNSb2lVRlY2M2lNbFE5MC9vU1N0ZTYrMGZnQkcxbEhjQTlvaHVBeG81WjV0bnRSVWZTZC9PUGcxL2lkUVJrdgo4Tkt6QUpockhFTmdqcml4aUpEN3dWMHJKNXk5VW1oMWxFTnlvZi9TYUZnakFtSkRITnJENC9mbHRaaWx3ZEZiClF0WjdMRGVrOGVzeE9RTmM3NmdVT2F4VlNuSE15MkZYQjZVT003U3Z0ZnMreGVnQllITXlpMTRoWWEvK0k1UDQKRFcxWTVHY0VVdUNpVDMxbmpXcCtCMXplcEU3TjZTQVlhYk5kNURqVWl3M1pjY3BIUjluUXZVbDZQd2hEZGtoNgo5MU5qVmg2dzEzYzRURHFCM08rQkdYbFNESWtPVk1iSU1PbXB5WmV0c3cxNlVyUjZKQnNOdVNTYnVzeStybkNXCjdlK2ZkazRlTTZkZ3htS0g3ZzY5elNVQ0F3RUFBUT09Ci0tLS0tRU5EIFBVQkxJQyBLRVktLS0tLQo\""
    "    }"
    "  ]"
    "}";
    char response[512];
        snprintf(response, sizeof(response),
                 "HTTP/1.1 200 OK\r\n"
                 "Content-Length: %lu\r\n\r\n%s",
                 (unsigned long)strlen(jwks_data), jwks_data);

        send(client_socket, response, strlen(response), 0);
    } else {
        send_response(client_socket, 404, "Not Found");
    }
}

void POST_request(int client_socket, const char* request) {
    if (strcmp(request, "/auth") == 0) {
    // JWT header and payload
    char *header_payload;
    sprintf(&header_payload, "{\"kid\":\"key-1\",\"use\":\"sig\"}");

    // Get the current time and create an expiration time, set to 1 hour
    time_t current_time = time(NULL);
    time_t expiration_time = current_time + 3600;

    // JWT payload
    char *payload;
    sprintf(&payload, "\"exp\":%ld", expiration_time);

    // Combine header and payload
    char *header_payload_b64 = base64_encode((unsigned char *)header_payload, strlen(header_payload));
    char *payload_b64 = base64_encode((unsigned char *)payload, strlen(payload));

    // Assemble the JWT
    char *jwt;
    sprintf(&jwt, "%s.%s", header_payload_b64, payload_b64);

    // Sign the JWT
    unsigned char *signature = (unsigned char *)malloc(RSA_size(rsa));
    unsigned int signature_length = 0;

    // Base64-encode the signature
    char *signature_b64 = base64_encode(signature, signature_length);

    // Append the signature to the JWT
    sprintf(&jwt, "%s.%s", jwt, signature_b64);

    free(header_payload);
    free(payload);
    free(header_payload_b64);
    free(payload_b64);
    free(signature);
    free(signature_b64);
        // Check for the "expired" query parameter
        const char* jwt_error = NULL;
        const char* query = strstr(request, "?expired");
        if (query != NULL) {
            // "expired" query parameter present, issue JWT signed with the expired key pair
            jwt = "JWT with Expired Key";  
        }
        char response_header[100];
        sprintf(response_header, "HTTP/1.1 200 OK\r\nContent-Length: %zu\r\n\r\n", strlen(jwt));
        send(client_socket, response_header, strlen(response_header), 0);
        send(client_socket, jwt, strlen(jwt), 0);
    } else {
        // Not found
        char not_found_response[] = "Not Found";
        char response_header[100];
        sprintf(response_header, "HTTP/1.1 404 Not Found\r\nContent-Length: %zu\r\n\r\n", strlen(not_found_response));
        send(client_socket, response_header, strlen(response_header), 0);
        send(client_socket, not_found_response, strlen(not_found_response), 0);
    }
}

int main() {

    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len = sizeof(client_addr);

    // Create socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Configure server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(SERVER_PORT);

    // Bind the socket
    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr) < 0)) {
        perror("Socket binding failed");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_socket, 10) == 0) {
        printf("Listening on port %d\n", SERVER_PORT);
    } else {
        perror("Server listening failed");
        exit(EXIT_FAILURE);
    }

    while (1) {
        client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_addr_len);
        if (client_socket < 0) {
            perror("Server accepting failed");
            continue;
        }

        char request_buffer[1024];
        recv(client_socket, request_buffer, sizeof(request_buffer), 0);
        printf("Received request:\n%s\n", request_buffer);

        // Parse the HTTP request to determine the method and path
        char request_method[10];
        char request_path[100];
        sscanf(request_buffer, "%s %s", request_method, request_path);

        if (strcmp(request_method, "GET") == 0) {
            GET_request(client_socket, request_path);
         } else if (strcmp(request_method, "POST") == 0) {
            POST_request(client_socket, request_path);
         }else {
            send_response(client_socket, 405, "Method Not Allowed");
        }

        close(client_socket);
    }

    close(server_socket);
    return 0;
}
