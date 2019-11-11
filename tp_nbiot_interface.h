/**
  * @file    tp_nbiot_interface.h
  * @version 0.1.0
  * @author  Adam Mitchell
  * @brief   Header file of the Thingpilot NB-IoT interface. This interface is hardware agnostic
  *          and depends on the underlying modem drivers exposing an identical interface
  */

/** Define to prevent recursive inclusion
 */
#pragma once

/** Includes 
 */
#include <mbed.h>
#include "board.h"

/** NB-IoT #defines 
 */
#define EARFCN_B8_LOW   3450
#define EARFCN_B8_HIGH  3799
#define EARFCN_B20_LOW  6150
#define EARFCN_B20_HIGH 6449


#if defined (BOARD) && (BOARD == WRIGHT_V1_0_0)

#include "SaraN2Driver.h"

#endif /* #if defined (BOARD) && (BOARD == WRIGHT_V1_0_0) */


class TP_NBIoT_Interface
{

	public:

		enum
		{
			UNDEFINED = 0,
			SARAN2    = 1
		};

		enum
		{
			NBIOT_OK         = 0,
			DRIVER_UNKNOWN   = 40
		};

	    #if defined (BOARD) && (BOARD == WRIGHT_V1_0_0 || BOARD == DEVELOPMENT_BOARD_V1_1_0)
		/** Constructor for the TP_NBIoT_Interface class, specifically when 
		 *  using a ublox Sara N2xx. Instantiates an ATCmdParser object
		 *  on the heap for comms between microcontroller and modem
		 * 
		 * @param txu Pin connected to SaraN2 TXD (This is MCU TXU)
		 * @param rxu Pin connected to SaraN2 RXD (This is MCU RXU)
		 * @param cts Pin connected to SaraN2 CTS
		 * @param rst Pin connected to SaraN2 RST
		 * @param vint Pin conencted to SaraN2 VINT
		 * @param gpio Pin connected to SaraN2 GPIO1
		 * @param baud Baud rate for UART between MCU and SaraN2
		 */  
		TP_NBIoT_Interface(PinName txu, PinName rxu, PinName cts, PinName rst, 
                           PinName vint, PinName gpio, int baud = 57600);

		#endif /* #if defined (BOARD) && (BOARD == ...) */

        /** Destructor for the TP_NBIoT_Interface class
         */
		~TP_NBIoT_Interface();

		/** Power-cycle the NB-IoT modem
		 * 
		 * @return Indicates success or failure reason
		 */
		int reboot_modem();

		/** Enable entire module Power Save Mode (PSM)
		 * 
		 * @return Indicates success or failure reason
		 */
		int enable_power_save_mode();

		/** Disable entire module Power Save Mode (PSM)
		 * 
		 * @return Indicates success or failure reason
		 */
		int disable_power_save_mode();

		/** Query UE for radio connection and network registration status
		 * 
		 * @param &connected Address of integer in which to store radio 
		 *                   connection status
		 * @param &reg_status Address of integer in which to store
		 *                    network registration status
		 * @return Indicates success or failure reason
		 */
        int get_connection_status(int &connected, int &reg_status);

		/** Return operation stats, of a given type, of the module
         * 
         * @param *data Point to .data parameter of Nuestats_t struct
         *              to copy data into
         * @return Indicates success or failure reason
         */
        int get_nuestats(char *data);

		/** Allow the platform to automatically attempt to connect to the 
		 *  network after power-on or reboot. Will set AT+CFUN=1 and read
		 *  the SIM PLMN. Will use APN provided by network.
		 * 
		 * @return Indicates success or failure reason
		 */
		int enable_autoconnect();

		/** Disable previously described autoconnect functionality
		 * 
		 * @return Indicates success or failure reason
		 */
		int disable_autoconnect();

		/** Enable CR_0354_0338 scrambling. This is an operator specific 
		 *  setting so please confirm with your mobile network provider
		 *  if you're unsure as to what to set this to
		 * 
		 * @return Indicates success or failure reason
		 */
		int enable_scrambling();

		/** Disable previously described CR_0354_0338 scrambling functionality
		 * 
		 * @return Indicates success or failure reason
		 */
		int disable_scrambling();

		/** Enable the scheduling of conflicted NSIB. This is an operator 
		 *  specific setting so please confirm with your mobile network 
		 *  provider if you're unsure as to what to set this to
		 * 
		 * @return Indicates success or failure reason
		 */
		int enable_si_avoid();

		/** Disable the previously described scheduling of conflicted NSIB
		 * 
		 * @return Indicates success or failure reason
		 */
		int disable_si_avoid();

		/** Enable combined EPS/IMSI network attach
		 * 
		 * @return Indicates success or failure reason
		 */
		int enable_combine_attach();

		/** Disable combined EPS/IMSI network attach
		 * 
		 * @return Indicates success or failure reason
		 */
		int disable_combine_attach();

		/** Enable RRC cell reselection
		 * 
		 * @return Indicates success or failure reason
		 */
		int enable_cell_reselection();

		/** Disable RRC cell reselection
		 * 
		 * @return Indicates success or failure reason
		 */
		int disable_cell_reselection();

		/** Enable Bearer Independent Protocol (BIP) where BIP is the interface
		 *  between the SIM and the ME which provides access to the data bearers
		 *  supported by the ME
		 * 
		 * @return Indicates success or failure reason
		 */
		int enable_bip();

		/** Disable previously described BIP functionality
		 * 
		 * @return Indicates success or failure reason
		 */
		int disable_bip();

		/** When enabled, the SIM is only powered when it is accessed and will be 
		 *  un-powered when not required, i.e. when in PSM
		 *
		 * @return Indicates success or failure reason
		 */
		int enable_sim_power_save_mode();

		/** Disable previously described SIM PSM
		 *
		 * @return Indicates success or failure reason
		 */
		int disable_sim_power_save_mode();

        /** Configure CoAP profile 0 with a given IP address, port and URI
         *
         * @param *ipv4 Pointer to a byte array storing the IPv4 address of the 
         *              destination server as a string, for example:
         *              char ipv4[] = "168.134.102.18"; 
         * @param port Destination server port
         * @param *uri Pointer to a byte array storing the URI, for example:
         *             char uri[] = "http://coap.me:5683/sink";
		 * @param uri_length Number of characters in URI, cannot be greater
 		 *                   than 200
         * @return Indicates success or failure reason
         */
		int configure_coap(char *ipv4, uint16_t port, char *uri, uint8_t uri_length);

		/** Perform a HTTP GET request over CoAP and capture the server
		 *  response in recv_data
		 *
		 * @param *recv_data Pointer to a byte array that will be populated
		 *              	 with the response from the server
         * @param &response_code Address of integer where CoAP operation response code
         *                       will be stored
		 * @return Indicates success or failure reason
		 */
		int coap_get(char *recv_data, int &response_code);

		/** Perform a HTTP DELETE request over CoAP and capture the server
		 *  response in recv_data
		 *
		 * @param *recv_data Pointer to a byte array that will be populated
		 *              	 with the response from the server
         * @param &response_code Address of integer where CoAP operation response code
         *                       will be stored
		 * @return Indicates success or failure reason
		 */
		int coap_delete(char *recv_data, int &response_code);

		/** Perform a PUT request using CoAP and save the returned 
		 *  data into recv_data
		 * 
		 * @param *send_data Pointer to a byte array containing the 
		 *                   data to be sent to the server
		 * @param *recv_data Pointer to a byte array where the data 
		 *                   returned from the server will be stored
		 * @param data_intenfier Integer value representing the data 
		 *                       format type. Possible values are enumerated
		 *                       in the header file, i.e. TEXT_PLAIN
         * @param &response_code Address of integer where CoAP operation response code
         *                       will be stored
		 * @return Indicates success or failure reason
		 */ 
		int coap_put(char *send_data, char *recv_data, int data_indentifier, int &response_code);

		/** Perform a POST request using CoAP and save the returned 
		 *  data into recv_data
		 * 
		 * @param *send_data Pointer to a byte array containing the 
		 *                   data to be sent to the server
		 * @param *recv_data Pointer to a byte array where the data 
		 *                   returned from the server will be stored
		 * @param data_intenfier Integer value representing the data 
		 *                       format type. Possible values are enumerated
		 *                       in the header file, i.e. TEXT_PLAIN
         * @param &response_code Address of integer where CoAP operation response code
         *                       will be stored
		 * @return Indicates success or failure reason
		 */ 
		int coap_post(char *send_data, char *recv_data, int data_indentifier, int &response_code);


	private:

		#if defined (_COMMS_NBIOT_DRIVER) && (_COMMS_NBIOT_DRIVER == SARAN2)
		SaraN2 _modem;
		#endif /* #if defined (_COMMS_NBIOT_DRIVER) && (_COMMS_NBIOT_DRIVER == SARAN2) */

		#if defined (_COMMS_NBIOT_DRIVER) && (_COMMS_NBIOT_DRIVER == SARAN2)
		int _driver = TP_NBIoT_Interface::SARAN2;
		#else 
		int _driver = TP_NBIoT_Interface::UNDEFINED;
		#endif /* #if defined (_COMMS_NBIOT_DRIVER) && (_COMMS_NBIOT_DRIVER == SARAN2) */
		
};


