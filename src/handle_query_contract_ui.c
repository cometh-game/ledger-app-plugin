#include "cometh_plugin.h"

// Set UI for "Beneficiary" screen.
static void set_beneficiary_ui(ethQueryContractUI_t *msg, context_t *context) {
    strlcpy(msg->title, "Beneficiary", msg->titleLength);

    // Prefix the address with `0x`.
    msg->msg[0] = '0';
    msg->msg[1] = 'x';

    // We need a random chainID for legacy reasons with `getEthAddressStringFromBinary`.
    // Setting it to `0` will make it work with every chainID :)
    uint64_t chainid = 0;

    // Get the string representation of the address stored in `context->beneficiary`. Put it in
    // `msg->msg`.
    getEthAddressStringFromBinary(
        context->beneficiary,
        msg->msg + 2,  // +2 here because we've already prefixed with '0x'.
        msg->pluginSharedRW->sha3,
        chainid);
}

// Set UI for Booster cards count
static void set_craft_booster_ui(ethQueryContractUI_t *msg, const context_t *context) {
    strlcpy(msg->title, "Booster", msg->titleLength);
    if (context->booster_card_count > 1) {
        snprintf(msg->msg, msg->msgLength, "%d cards", context->booster_card_count);
    } else {
        snprintf(msg->msg, msg->msgLength, "%d card", context->booster_card_count);
    }
    // snprintf(msg->msg, msg->msgLength, "%d card(s)", context->booster_card_count);
}

// Set UI for the "Send" screen.
static void set_action_ui(ethQueryContractUI_t *msg, const context_t *context) {
    switch (context->selectorIndex) {
        //case CRAFT:
        //    strlcpy(msg->title, "Booster", msg->titleLength);
        //    amountToString(context->recipe_id, 1, WEI_TO_ETHER, "Cards ", msg->msg, msg->msgLength);
        //    break;
        //case REDEEM:
            //strlcpy(msg->title, "Booster", msg->titleLength);
            ///amountToString(context->lootbox_id, 1, WEI_TO_ETHER, "", msg->msg, msg->msgLength);
            //break;
        case GRIND:
            strlcpy(msg->title, "Booster", msg->titleLength);
            break;
        default:
            PRINTF("Unhandled selector Index: %d\n", context->selectorIndex);
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            return;
    }

    /*
    strlcpy(msg->title, "Send", msg->titleLength);

    const uint8_t *eth_amount = msg->pluginSharedRO->txContent->value.value;
    uint8_t eth_amount_size = msg->pluginSharedRO->txContent->value.length;

    // Converts the uint256 number located in `eth_amount` to its string representation and
    // copies this to `msg->msg`.
    amountToString(eth_amount, eth_amount_size, WEI_TO_ETHER, "ETH ", msg->msg, msg->msgLength);
    */
}

/*
// Set UI for "Receive" screen.
static void set_receive_ui(ethQueryContractUI_t *msg, const context_t *context) {
    strlcpy(msg->title, "Receive Min.", msg->titleLength);

    uint8_t decimals = context->decimals;
    const char *ticker = context->ticker;

    // If the token look up failed, use the default network ticker along with the default decimals.
    if (!context->token_found) {
        decimals = WEI_TO_ETHER;
        ticker = msg->network_ticker;
    }

    amountToString(context->amount_received,
                   sizeof(context->amount_received),
                   decimals,
                   ticker,
                   msg->msg,
                   msg->msgLength);
}
*/

/*
// Helper function that returns the enum corresponding to the screen that should be displayed.
static screens_t get_screen(const ethQueryContractUI_t *msg,
                            const context_t *context __attribute__((unused))) {
    uint8_t index = msg->screenIndex;
    switch (index) {
        case 0:
            return CRAFT_BOOSTER_SCREEN;
        //case 1:
        //    return REDEEM_SCREEN;
        case 1:
            return BENEFICIARY_SCREEN;
        default:
            return ERROR;
    }
}
*/

void handle_query_contract_ui(void *parameters) {
    ethQueryContractUI_t *msg = (ethQueryContractUI_t *) parameters;
    context_t *context = (context_t *) msg->pluginContext;

    // Clean the display fields.
    memset(msg->title, 0, msg->titleLength);
    memset(msg->msg, 0, msg->msgLength);
    msg->result = ETH_PLUGIN_RESULT_OK;

    // screens_t screen = get_screen(msg, context);
    // PRINTF("Switching to selector: %d and screen: %d\n", context->selectorIndex, screen);
    PRINTF("Switching to selector: %d and screen: %d\n", context->selectorIndex, msg->screenIndex);

    switch (context->selectorIndex) {
        case CRAFT:
            switch (msg->screenIndex) {
                case 0:
                    set_beneficiary_ui(msg, context);
                    return;
                case 1:
                    set_craft_booster_ui(msg, context);
                    return;
            }
        case REDEEM:
            switch (msg->screenIndex) {
                case 0:
                    set_beneficiary_ui(msg, context);
                    return;
            }
    }

    PRINTF("Received an invalid selectorIndex + screenIndex tuple\n");
    msg->result = ETH_PLUGIN_RESULT_ERROR;
    return;
}
