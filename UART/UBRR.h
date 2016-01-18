#define UBRR_1_9600_0 6U
#define UBRR_1_9600_1 12U
#define UBRR_1_19200_0 2U
#define UBRR_1_19200_1 6U

#define UBRR_8_9600_0 51U
#define UBRR_8_9600_1 103U
#define UBRR_8_19200_0 25U
#define UBRR_8_19200_1 51U

#define UBRR_16_9600_0 103U
#define UBRR_16_9600_1 207U
#define UBRR_16_19200_0 51U
#define UBRR_16_19200_1 103U

#define UBRR_20_9600_0 129U
#define UBRR_20_9600_1 259U
#define UBRR_20_19200_0 64U
#define UBRR_20_19200_1 129U

#define _UBRR(fcpu, baud, u2x) UBRR_##fcpu##_##baud##_##u2x