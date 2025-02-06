# name_testing_artifacts

## Description
In this repo, we can provide artifacts, test cases, and some additional results (due to lack of space in the paper). 

## Summary of the results of KLEE with meta-level search

|      Impl      |  Task  | Parh explored | Accepting path |  Exec time  |
| :------------: | :----: | :-----------: | :------------: | :---------: |
|    BearSSL     | HV-SAN |    104766     |     12376      | 2h 16m 51s  |
|                | HV-CN  |    115353     |     26592      | 17d 13h 45m |
|   MatrixSSL    | HV-SAN |     1084      |       99       |   2m 55s    |
|                | HV-CN  |      791      |       74       |   19m 46s   |
|    MbedTLS     |   NC   |     2949      |      1063      | 1h 43m 41s  |
|                | HV-SAN |     2786      |      663       |   21m 56s   |
|                | HV-CN  |     12716     |      3192      | 16h 15m 36s |
|    WolfSSL     | HV-SAN |     71709     |     12121      |   13m 38s   |
|    OpenSSL     |   NC   |    1867952    |      2560      |  over 30d   |
|                | HV-SAN |    592287     |      9804      | 1d 21h 50m  |
|                | HV-CN  |    2616745    |     303512     | 18d 7h 19m  |
|   StrongSwan   |   NC   |      525      |       25       |   15m 39s   |
| WPA Supplicant |   NC   |    234760     |     25329      |   28m 16s   |
|     GnuTLS     | HV-SAN |     21373     |      2415      | 4h 21m 28s  |
|                | HV-CN  |    7040721    |     16133      | 14d 17h 1m  |

## Implementations and classes/functions related to name chaining and hostname verification.
|         Impl        | Lang |          Class / FunctionName         |                                    |
|:-------------------:|:----:|:-------------------------------------:|:----------------------------------:|
|                     |      |             Name Chaining             |        Hostname verification       |
|        axTLS        |   C  |               x509_verify             |                 N/A                |
|       BearSSL       |   C  |          start_chain end_chain        |         start_chain end_chain      |
|      BoringSSL      |   C  |             x509_verify_cert          |            x509_check_host         |
|        Botan        |  Cpp |            x509_path_validate         |          x509_path_validate        |
|        GnuTLS       |   C  |     gnutls_x509_trust_list.verify_crt |     gnutls_x509_crt_check_hostname |
|      MatrixSSL      |   C  |          matrixValidateCerts          |         matrixValidateCerts        |
|       MbedTLS       |   C  |          mbedtls_x509_crt_verify      |        mbedtls_x509_crt_verify     |
| OpenSSL  OpenSSL v3 |   C  |             X509_verify_cert          |            X509_check_host         |
|       WolfSSL       |   C  |        wolfSSL_CertManagerVerify      |        wolfSSL_X509_check_host     |
|      StrongSwan     |   C  |            verify_trust_chain         |                 N/A                |
|         WPA         |   C  |      x509_certificate_chain_validate  |                 N/A                |
|        Webpki       | Rust |       verify_is_valid_tls_server_cert |       verify_is_valid_for_dns_name |
|    certvalidator    |  Py  |          CertificateValidator         |                 N/A                |
|         ssl         |  Py  |                  N/A                  |            match_hostname          |
|        Crypto       |  Go  |           cert.Verify(opts)           |         x509.VerifyHostname        |
|                     |      |                                       |             x509.Verify            |
|         OTP         |  Erl |           pkix_path_validation        |        pkix_verify_hostname        |
|    Bouncy Castle    | Java |           CertPathValidator           |    JsseDefaultHostnameAuthorizer   |
|       SunJSSE       | Java |           CertPathValidator           |           HostnameChecker          |
|  Apache-HttpClient  | Java |                  N/A                  |       DefaultHostnameVerifier      |
|      Apache-CXF     | Java |                  N/A                  |       DefaultHostnameVerifier      |
|        seclib       |  PHP |               getChain()              |             validateURL            |
|         Node        |  JS  |                  N/A                  |           x509.checkHost           |
|                     |      |                                       |         checkServerIdentity        |
|        forge        |  JS  |       pki.verifyCertificateChain      |                 N/A                |

## Full code coverage table

### NC 

|  Library Task  	| Coverage 	|     Approach    	|                 	|             	|             	  |               |             	|
|:--------------:|:--------:|:---------------:|:---------------:|:-----------:|:-----------:|:-----------:|:-----------:|
|                	|          	| Frankencert(2M) 	| Frankencert(8M) 	|    Craft    	|     Fuzz    	|     KMLS    	|   C+F+KMLS  	|
|   GnuTLS (NC)  	|   Line   	|   3211(4.2\%)   	|   3211(4.2\%)   	| 3407(4.5\%) 	| 3504(4.6\%) 	|     N/A     	| 3530(4.6\%) 	|
|                	|   Func   	|    267(6.3\%)   	|    267(6.3\%)   	|  278(6.6\%) 	|  285(6.7\%) 	|     N/A     	|  285(6.7\%) 	|
|                	|  Branch  	|   1643(3.0\%)   	|   1643(3.0\%)   	| 1728(3.1\%) 	| 1783(3.2\%) 	|     N/A     	| 1812(3.3\%) 	|
| Matrixssl (NC) 	|   Line   	|    245(0.7\%)   	|    245(0.7\%)   	| 1599(4.9\%) 	| 1633(5.0\%) 	|     N/A     	| 1684(5.1\%) 	|
|                	|   Func   	|    17(1.0\%)    	|    17(1.0\%)    	|  112(6.5\%) 	|  112(6.5\%) 	|     N/A     	|  113(6.5\%) 	|
|                	|  Branch  	|    118(0.5\%)   	|    118(0.5\%)   	|  778(3.5\%) 	|  804(3.7\%) 	|     N/A     	|  835(3.8\%) 	|
|  MbedTLS (NC)  	|   Line   	|   1613(3.0\%)   	|   1613(3.0\%)   	| 1540(2.9\%) 	| 1554(2.9\%) 	| 1468(2.7\%) 	| 1592(3.0\%) 	|
|                	|   Func   	|    148(4.5\%)   	|    148(4.5\%)   	|  145(4.4\%) 	|  147(4.5\%) 	|  143(4.4\%) 	|  147(4.5\%) 	|
|                	|  Branch  	|    781(2.0\%)   	|    781(2.0\%)   	|  818(2.0\%) 	|  812(2.0\%) 	|  763(1.9\%) 	|  852(2.1\%) 	|
|  OpenSSL (NC)  	|   Line   	|   6324(4.2\%)   	|   6324(4.2\%)   	| 5322(3.5\%) 	| 6419(4.2\%) 	| 6334(4.2\%) 	| 6565(4.3\%) 	|
|                	|   Func   	|    887(8.2\%)   	|    887(8.2\%)   	|  796(7.3\%) 	|  902(8.3\%) 	|  892(8.3\%) 	|  903(8.3\%) 	|
|                	|  Branch  	|   2745(3.0\%)   	|   2745(3.0\%)   	| 2291(2.5\%) 	| 2755(3.0\%) 	| 2711(3.0\%) 	| 2841(3.1\%) 	|

### HV-SAN

|      Library Task      	| Coverage 	|   Approach  	|             	|             	|             	|             	|
|:----------------------:    |:--------:    |:-----------:    |:-----------:    |:-----------:    |:-----------:    |:-----------:    |
|                            |              |   HVLearn       |    Craft        |     Fuzz        |     KMLS        |   C+F+KMLS      |
|     GnuTLS (HV-SAN)       |   Line       | 1820(2.4\%)     | 1799(2.4\%)     | 1819(2.4\%)     | 2115(2.8\%)     | 2116(2.8\%)     |
|                            |   Func       |  146(3.5\%)     |  146(3.5\%)     |  146(3.5\%)     |  162(3.8\%)     |  162(3.8\%)     |
|                            |  Branch      |  974(1.7\%)     |  947(1.7\%)     |  970(1.7\%)     | 1138(2.0\%)     | 1141(2.0\%)     |
|  Matrixssl (HV-SAN)       |   Line       | 1516(4.6\%)     | 1124(3.4\%)     | 1115(3.4\%)     | 1516(4.6\%)     | 1614(4.9\%)     |
|                            |   Func       |  110(6.4\%)     |  76(4.4\%)      |  76(4.4\%)      |  110(6.4\%)     |  113(6.5\%)     |
|                            |  Branch      |  726(3.3\%)     |  608(2.8\%)     |  608(2.8\%)     |  727(3.3\%)     |  793(3.6\%)     |
|    MbedTLS (HV-SAN)          |   Line       |  686(1.3\%)     |  696(1.3\%)     |  695(1.3\%)     |  686(1.3\%)     |  698(1.3\%)     |
|                            |   Func       |  68(2.1\%)      |  68(2.1\%)      |  68(2.1\%)      |  68(2.1\%)      |  68(2.1\%)      |
|                            |  Branch      |  386(1.0\%)     |  393(1.0\%)     |  391(1.0\%)     |  386(1.0\%)     |  395(1.0\%)     |
|    OpenSSL (HV-SAN)       |   Line       | 3703(2.4\%)     | 3688(2.4\%)     | 3710(2.5\%)     | 3732(2.5\%)     | 3749(2.5\%)     |
|                            |   Func       |  592(5.5\%)     |  593(5.5\%)     |  593(5.5\%)     |  593(5.5\%)     |  594(5.5\%)     |
|                            |  Branch      | 1540(1.7\%)     | 1489(1.6\%)     | 1530(1.7\%)     | 1579(1.7\%)     | 1588(1.7\%)     |

### HV-CN

|     Library Task         | Coverage     |   Approach      |                 |                 |                 |                 |
|:--------------------:    |:--------:    |:-----------:    |:-----------:    |:-----------:    |:-----------:    |:-----------:    |
|                          |              |   HVLearn       |    Craft        |     Fuzz        |     KMLS        |   C+F+KMLS      |
|    GnuTLS (HV-CN)        |   Line       | 1937(2.5\%)     | 1869(2.4\%)     | 1952(2.6\%)     | 2400(3.1\%)     | 2405(3.1\%)     |
|                          |   Func       |  151(3.6\%)     |  145(3.4\%)     |  151(3.6\%)     |  173(4.1\%)     |  173(4.1\%)     |
|                          |  Branch      | 1030(1.9\%)     |  995(1.8\%)     | 1045(1.9\%)     | 1303(2.3\%)     | 1306(2.3\%)     |
| Matrixssl (HV-CN)     |   Line       | 1024(2.1\%)     |  959(2.9\%)     | 1027(3.1\%)     | 1024(3.1\%)     | 1028(3.1\%)     |
|                          |   Func       |  73(4.2\%)      |  70(4.0\%)      |  73(4.2\%)      |  73(4.2\%)      |  73(4.2\%)      |
|                          |  Branch      |  556(2.5\%)     |  519(2.4\%)     |  557(2.5\%)     |  562(2.6\%)     |  564(2.6\%)     |
|    MbedTLS (HV-CN)       |   Line       |  662(1.2\%)     |  660(1.2\%)     |  665(1.2\%)     |  715(1.3\%)     |  720(1.3\%)     |
|                          |   Func       |  66(2.0\%)      |  66(2.0\%)      |  66(2.0\%)      |  69(2.1\%)      |  69(2.1\%)      |
|                          |  Branch      |  377(0.9\%)     |  375(0.9\%)     |  377(0.9\%)     |  410(1.0\%)     |  413(1.0\%)     |
|    OpenSSL (HV-CN)       |   Line       | 3680(2.4\%)     | 3627(2.4\%)     | 3641(2.4\%)     | 3926(2.6\%)     | 3939(2.6\%)     |
|                          |   Func       |  587(5.4\%)     |  585(5.4\%)     |  584(5.4\%)     |  593(5.5\%)     |  593(5.5\%)     |
|                          |  Branch      | 1524(1.7\%)     | 1469(1.6\%)     | 1479(1.6\%)     | 1699(1.9\%)     | 1708(1.9\%)     |
