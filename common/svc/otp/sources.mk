INC += -I../common/svc/otp/mbedtls/include
SRC += ../common/svc/otp/mbedtls/library/sha1.c \
       ../common/svc/otp/memxor.c \
       ../common/svc/otp/hmac-sha1.c \
       ../common/svc/otp/hotp.c \
       ../common/svc/otp/totp.c

DEFINES += -DMBEDTLS_CONFIG_FILE=\"../common/svc/otp/mbedtls/mbedtls_config.h\"
