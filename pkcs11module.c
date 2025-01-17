/*
   +----------------------------------------------------------------------+
   | PHP Version 7                                                        |
   +----------------------------------------------------------------------+
   | Copyright (c) The PHP Group                                          |
   +----------------------------------------------------------------------+
   | This source file is subject to version 3.01 of the PHP license,      |
   | that is bundled with this package in the file LICENSE, and is        |
   | available through the world-wide-web at the following url:           |
   | http://www.php.net/license/3_01.txt                                  |
   | If you did not receive a copy of the PHP license and are unable to   |
   | obtain it through the world-wide-web, please send a note to          |
   | license@php.net so we can mail you a copy immediately.               |
   +----------------------------------------------------------------------+
   | Author: Guillaume Amringer                                           |
   +----------------------------------------------------------------------+
*/

#include "pkcs11int.h"

zend_class_entry *ce_Pkcs11_Module;
static zend_object_handlers pkcs11_handlers;


ZEND_BEGIN_ARG_INFO_EX(arginfo___construct, 0, 0, 1)
    ZEND_ARG_TYPE_INFO(0, modulePath, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_getInfo, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_C_GetInfo, 0, 0, 1)
    ZEND_ARG_TYPE_INFO(1, pInfo, IS_ARRAY, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_getSlots, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_getSlotList, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_C_GetSlotList, 0, 0, 2)
    ZEND_ARG_TYPE_INFO(0, tokenPresent, _IS_BOOL, 0)
    ZEND_ARG_TYPE_INFO(1, pSlotList, IS_ARRAY, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_getSlotInfo, 0, 0, 1)
    ZEND_ARG_TYPE_INFO(0, slotId, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_C_GetSlotInfo, 0, 0, 2)
    ZEND_ARG_TYPE_INFO(0, slotId, IS_LONG, 0)
    ZEND_ARG_TYPE_INFO(1, pInfo, IS_ARRAY, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_getTokenInfo, 0, 0, 1)
    ZEND_ARG_TYPE_INFO(0, slotId, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_C_GetTokenInfo, 0, 0, 2)
    ZEND_ARG_TYPE_INFO(0, slotId, IS_LONG, 0)
    ZEND_ARG_TYPE_INFO(1, pInfo, IS_ARRAY, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_getMechanismList, 0, 0, 1)
    ZEND_ARG_TYPE_INFO(0, slotId, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_C_GetMechanismList, 0, 0, 2)
    ZEND_ARG_TYPE_INFO(0, slotId, IS_LONG, 0)
    ZEND_ARG_TYPE_INFO(1, pMechanismList, IS_ARRAY, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_getMechanismInfo, 0, 0, 2)
    ZEND_ARG_TYPE_INFO(0, slotId, IS_LONG, 0)
    ZEND_ARG_TYPE_INFO(0, mechanismId, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_C_GetMechanismInfo, 0, 0, 3)
    ZEND_ARG_TYPE_INFO(0, slotId, IS_LONG, 0)
    ZEND_ARG_TYPE_INFO(0, type, IS_LONG, 0)
    ZEND_ARG_TYPE_INFO(1, pInfo, IS_ARRAY, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_initToken, 0, 0, 3)
    ZEND_ARG_TYPE_INFO(0, slotid, IS_LONG, 0)
    ZEND_ARG_TYPE_INFO(0, label, IS_STRING, 0)
    ZEND_ARG_TYPE_INFO(0, sopin, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_C_InitToken, 0, 0, 3)
    ZEND_ARG_TYPE_INFO(0, slotid, IS_LONG, 0)
    ZEND_ARG_TYPE_INFO(0, label, IS_STRING, 0)
    ZEND_ARG_TYPE_INFO(0, sopin, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_openSession, 0, 0, 1)
    ZEND_ARG_TYPE_INFO(0, slotid, IS_LONG, 0)
    ZEND_ARG_TYPE_INFO(0, flags, IS_LONG, 0)
    ZEND_ARG_TYPE_INFO(0, application, IS_STRING, 1)
    ZEND_ARG_TYPE_INFO(0, notify, IS_CALLABLE, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_C_OpenSession, 0, 0, 5)
    ZEND_ARG_TYPE_INFO(0, slotID, IS_LONG, 0)
    ZEND_ARG_TYPE_INFO(0, flags, IS_LONG, 0)
    ZEND_ARG_TYPE_INFO(0, pApplication, IS_STRING, 1)
    ZEND_ARG_TYPE_INFO(0, notify, IS_CALLABLE, 1)
    ZEND_ARG_OBJ_INFO(1, hSession, Pkcs11\\Session, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_C_WaitForSlotEvent, 0, 0, 2)
    ZEND_ARG_TYPE_INFO(0, php_flags, IS_LONG, 1)
    ZEND_ARG_TYPE_INFO(1, php_slotID, IS_LONG, 1)
ZEND_END_ARG_INFO()


ZEND_BEGIN_ARG_INFO_EX(arginfo_waitForSlotEvent, 0, 0, 1)
    ZEND_ARG_TYPE_INFO(0, php_flags, IS_LONG, 1)
ZEND_END_ARG_INFO()


PHP_METHOD(Module, __construct) {
    char *module_path;
    size_t module_path_len;

    ZEND_PARSE_PARAMETERS_START(1, 1)
        Z_PARAM_PATH(module_path, module_path_len)
    ZEND_PARSE_PARAMETERS_END();


    pkcs11_object *objval = Z_PKCS11_P(ZEND_THIS);

    if (objval->initialised) {
        zend_throw_exception(zend_ce_exception, "Already initialised PKCS11 module", 0);
        return;
    }

    CK_RV rv;

    char* dlerror_str;
    objval->pkcs11module = dlopen(module_path, RTLD_NOW);

    dlerror_str = dlerror();
    if (dlerror_str != NULL) {
        general_error("Unable to initialise PKCS11 module", dlerror_str);
        return;
    }

    CK_C_GetFunctionList C_GetFunctionList = dlsym(objval->pkcs11module, "C_GetFunctionList");
    dlerror_str = dlerror();
    if (dlerror_str != NULL) {
        general_error("Unable to initialise PKCS11 module", dlerror_str);
        return;
    }

    rv = C_GetFunctionList(&objval->functionList);
    if (rv != CKR_OK) {
        pkcs11_error(rv, "Unable to retrieve function list");
        return;
    }

    rv = objval->functionList->C_Initialize(NULL);
    if (rv != CKR_OK) {
        pkcs11_error(rv, "Unable to initialise token");
        return;
    }

    objval->initialised = true;
}


CK_RV php_C_GetInfo(pkcs11_object *objval, zval *retval) {

    CK_RV rv;
    CK_INFO info;

    rv = objval->functionList->C_GetInfo(&info);
    if (rv != CKR_OK) {
        return rv;
    }

    zval cryptokiversion;
    array_init(&cryptokiversion);
    add_assoc_long(&cryptokiversion, "major", info.cryptokiVersion.major);
    add_assoc_long(&cryptokiversion, "minor", info.cryptokiVersion.minor);

    zval libversion;
    array_init(&libversion);
    add_assoc_long(&libversion, "major", info.libraryVersion.major);
    add_assoc_long(&libversion, "minor", info.libraryVersion.minor);

    array_init(retval);
    add_assoc_zval(retval, "cryptokiVersion", &cryptokiversion);
    add_assoc_stringl(retval, "manufacturerID", info.manufacturerID, sizeof(info.manufacturerID));
    add_assoc_stringl(retval, "libraryDescription", info.libraryDescription, sizeof(info.libraryDescription));
    add_assoc_zval(retval, "libraryVersion", &libversion);

    return rv;
}

/* {{{ proto array Pkcs11\Module::getInfo(void)
   Return the session cookie parameters */
PHP_METHOD(Module, getInfo) {
    pkcs11_object *objval = Z_PKCS11_P(ZEND_THIS);

    ZEND_PARSE_PARAMETERS_NONE();

    if (!objval->initialised) {
        zend_throw_exception(zend_ce_exception, "Uninitialised PKCS11 module", 0);
        return;
    }

    CK_RV rv = php_C_GetInfo(objval, return_value);
    if (rv != CKR_OK) {
        pkcs11_error(rv, "Unable to get information from token");
    }
}
/* }}} */

PHP_METHOD(Module, C_GetInfo) {
    CK_RV rv;
    zval *pInfo;
    zval retval;

    ZEND_PARSE_PARAMETERS_START(1, 1)
        Z_PARAM_ZVAL(pInfo)
    ZEND_PARSE_PARAMETERS_END();

    pkcs11_object *objval = Z_PKCS11_P(ZEND_THIS);

    if (!objval->initialised) {
        zend_throw_exception(zend_ce_exception, "Uninitialised PKCS11 module", 0);
        return;
    }

    rv = php_C_GetInfo(objval, &retval);

    ZEND_TRY_ASSIGN_REF_VALUE(pInfo, &retval);

    RETURN_LONG(rv);
}

CK_RV php_C_GetSlotList(pkcs11_object *objval, zend_bool tokenPresent, zval *retval) {

    CK_RV rv;
    CK_ULONG ulSlotCount;
    CK_SLOT_ID_PTR pSlotList;

    rv = objval->functionList->C_GetSlotList((CK_BBOOL)tokenPresent, NULL_PTR, &ulSlotCount);
    if (rv != CKR_OK) {
        return rv;
    }

    pSlotList = (CK_SLOT_ID_PTR) ecalloc(ulSlotCount, sizeof(CK_SLOT_ID));
    rv = objval->functionList->C_GetSlotList((CK_BBOOL)tokenPresent, pSlotList, &ulSlotCount);
    if (rv != CKR_OK) {
        efree(pSlotList);
        return rv;
    }

    array_init(retval);
    for (CK_SLOT_ID i=0; i<ulSlotCount; i++) {
        add_next_index_long(retval, pSlotList[i]);
    }

    efree(pSlotList);

    return rv;
}

PHP_METHOD(Module, getSlots) {
    CK_RV rv;
    CK_ULONG ulSlotCount;
    CK_SLOT_ID_PTR pSlotList;
    CK_SLOT_INFO slotInfo;

    ZEND_PARSE_PARAMETERS_NONE();

    pkcs11_object *objval = Z_PKCS11_P(ZEND_THIS);

    if (!objval->initialised) {
        zend_throw_exception(zend_ce_exception, "Uninitialised PKCS11 module", 0);
        return;
    }

    rv = objval->functionList->C_GetSlotList(CK_FALSE, NULL_PTR, &ulSlotCount);
    if (rv != CKR_OK) {
        pkcs11_error(rv, "Unable to get slot list from token");
        return;
    }

    pSlotList = (CK_SLOT_ID_PTR) ecalloc(ulSlotCount, sizeof(CK_SLOT_ID));
    rv = objval->functionList->C_GetSlotList(CK_FALSE, pSlotList, &ulSlotCount);
    if (rv != CKR_OK) {
        efree(pSlotList);
        pkcs11_error(rv, "Unable to get slot list from token");
        return;
    }

    zval slotObj;
    array_init(return_value);
    for (CK_SLOT_ID i=0; i<ulSlotCount; i++) {
        rv = objval->functionList->C_GetSlotInfo(pSlotList[i], &slotInfo);
        if (rv != CKR_OK) {
            pkcs11_error(rv, "Unable to get slot info from token");
            return;
        }

        array_init(&slotObj);
        add_assoc_long(&slotObj, "id", pSlotList[i]);
        add_assoc_stringl(&slotObj, "slotDescription", slotInfo.slotDescription, sizeof(slotInfo.slotDescription));
        add_index_zval(return_value, pSlotList[i], &slotObj);
    }
    efree(pSlotList);
}

PHP_METHOD(Module, getSlotList) {

    ZEND_PARSE_PARAMETERS_NONE();

    pkcs11_object *objval = Z_PKCS11_P(ZEND_THIS);

    if (!objval->initialised) {
        zend_throw_exception(zend_ce_exception, "Uninitialised PKCS11 module", 0);
        return;
    }

    CK_RV rv = php_C_GetSlotList(objval, false, return_value);
    if (rv != CKR_OK) {
        pkcs11_error(rv, "Unable to get slot list from token");
    }
}

PHP_METHOD(Module, C_GetSlotList) {
    CK_RV rv;
    zend_bool tokenPresent;
    zval *pSlotList;
    zval retval;

    ZEND_PARSE_PARAMETERS_START(2, 2)
        Z_PARAM_BOOL(tokenPresent)
        Z_PARAM_ZVAL(pSlotList)
    ZEND_PARSE_PARAMETERS_END();

    pkcs11_object *objval = Z_PKCS11_P(ZEND_THIS);

    if (!objval->initialised) {
        zend_throw_exception(zend_ce_exception, "Uninitialised PKCS11 module", 0);
        return;
    }

    rv = php_C_GetSlotList(objval, tokenPresent, &retval);

    ZEND_TRY_ASSIGN_REF_VALUE(pSlotList, &retval);

    RETURN_LONG(rv);
}


CK_RV php_C_GetSlotInfo(pkcs11_object *objval, zend_long slotId, zval *retval) {

    CK_RV rv;
    CK_SLOT_INFO slotInfo;

    rv = objval->functionList->C_GetSlotInfo((CK_SLOT_ID)slotId, &slotInfo);
    if (rv != CKR_OK) {
        return rv;
    }

    array_init(retval);
    add_assoc_long(retval, "id", (CK_SLOT_ID)slotId);
    add_assoc_stringl(retval, "description", slotInfo.slotDescription, 64);
    add_assoc_stringl(retval, "manufacturerID", slotInfo.manufacturerID, sizeof(slotInfo.manufacturerID));
    add_assoc_long(retval, "flags", slotInfo.flags);

    zval hardwareVersion;
    array_init(&hardwareVersion);
    add_assoc_long(&hardwareVersion, "major", slotInfo.hardwareVersion.major);
    add_assoc_long(&hardwareVersion, "minor", slotInfo.hardwareVersion.minor);
    add_assoc_zval(retval, "hardwareVersion", &hardwareVersion);

    zval firmwareVersion;
    array_init(&firmwareVersion);
    add_assoc_long(&firmwareVersion, "major", slotInfo.firmwareVersion.major);
    add_assoc_long(&firmwareVersion, "minor", slotInfo.firmwareVersion.minor);
    add_assoc_zval(retval, "firmwareVersion", &firmwareVersion);

    return rv;
}

PHP_METHOD(Module, getSlotInfo) {

    zend_long slotId;

    ZEND_PARSE_PARAMETERS_START(1, 1)
        Z_PARAM_LONG(slotId)
    ZEND_PARSE_PARAMETERS_END();

    pkcs11_object *objval = Z_PKCS11_P(ZEND_THIS);

    if (!objval->initialised) {
        zend_throw_exception(zend_ce_exception, "Uninitialised PKCS11 module", 0);
        return;
    }


    CK_RV rv = php_C_GetSlotInfo(objval, slotId, return_value);
    if (rv != CKR_OK) {
        pkcs11_error(rv, "Unable to get slot info from token");
    }
}

PHP_METHOD(Module, C_GetSlotInfo) {
    CK_RV rv;
    zend_long slotId;
    zval *pInfo;
    zval retval;

    ZEND_PARSE_PARAMETERS_START(2, 2)
        Z_PARAM_LONG(slotId)
        Z_PARAM_ZVAL(pInfo)
    ZEND_PARSE_PARAMETERS_END();

    pkcs11_object *objval = Z_PKCS11_P(ZEND_THIS);

    if (!objval->initialised) {
        zend_throw_exception(zend_ce_exception, "Uninitialised PKCS11 module", 0);
        return;
    }

    rv = php_C_GetSlotInfo(objval, slotId, &retval);

    ZEND_TRY_ASSIGN_REF_VALUE(pInfo, &retval);

    RETURN_LONG(rv);
}


CK_RV php_C_GetTokenInfo(pkcs11_object *objval, CK_SLOT_ID slotId, zval *retval) {

    CK_RV rv;
    CK_TOKEN_INFO tokenInfo = {};

    rv = objval->functionList->C_GetTokenInfo((CK_SLOT_ID)slotId, &tokenInfo);
    if (rv != CKR_OK) {
        return rv;
    }

    array_init(retval);
    add_assoc_stringl(retval, "label", tokenInfo.label, sizeof(tokenInfo.label));
    add_assoc_stringl(retval, "manufacturerID", tokenInfo.manufacturerID, sizeof(tokenInfo.manufacturerID));
    add_assoc_stringl(retval, "model", tokenInfo.model, sizeof(tokenInfo.model));
    add_assoc_stringl(retval, "serialNumber", tokenInfo.serialNumber, sizeof(tokenInfo.serialNumber));

    add_assoc_long(retval, "flags", tokenInfo.flags);

    add_assoc_long(retval, "ulMaxSessionCount", tokenInfo.ulMaxSessionCount);
    add_assoc_long(retval, "ulSessionCount", tokenInfo.ulSessionCount);
    add_assoc_long(retval, "ulMaxRwSessionCount", tokenInfo.ulMaxRwSessionCount);
    add_assoc_long(retval, "ulRwSessionCount", tokenInfo.ulRwSessionCount);
    add_assoc_long(retval, "ulMaxPinLen", tokenInfo.ulMaxPinLen);
    add_assoc_long(retval, "ulMinPinLen", tokenInfo.ulMinPinLen);
    add_assoc_long(retval, "ulTotalPublicMemory", tokenInfo.ulTotalPublicMemory);
    add_assoc_long(retval, "ulFreePublicMemory", tokenInfo.ulFreePublicMemory);
    add_assoc_long(retval, "ulTotalPrivateMemory", tokenInfo.ulTotalPrivateMemory);
    add_assoc_long(retval, "ulFreePrivateMemory", tokenInfo.ulFreePrivateMemory);

    zval hardwareVersion;
    array_init(&hardwareVersion);
    add_assoc_long(&hardwareVersion, "major", tokenInfo.hardwareVersion.major);
    add_assoc_long(&hardwareVersion, "minor", tokenInfo.hardwareVersion.minor);
    add_assoc_zval(retval, "hardwareVersion", &hardwareVersion);

    zval firmwareVersion;
    array_init(&firmwareVersion);
    add_assoc_long(&firmwareVersion, "major", tokenInfo.firmwareVersion.major);
    add_assoc_long(&firmwareVersion, "minor", tokenInfo.firmwareVersion.minor);
    add_assoc_zval(retval, "firmwareVersion", &firmwareVersion);

    add_assoc_stringl(retval, "utcTime", tokenInfo.utcTime, sizeof(tokenInfo.utcTime));

    return rv;
}

PHP_METHOD(Module, getTokenInfo) {

    zend_long slotId;

    ZEND_PARSE_PARAMETERS_START(1, 1)
        Z_PARAM_LONG(slotId)
    ZEND_PARSE_PARAMETERS_END();

    pkcs11_object *objval = Z_PKCS11_P(ZEND_THIS);

    if (!objval->initialised) {
        zend_throw_exception(zend_ce_exception, "Uninitialised PKCS11 module", 0);
        return;
    }

    CK_RV rv = php_C_GetTokenInfo(objval, slotId, return_value);
    if (rv != CKR_OK) {
        pkcs11_error(rv, "Unable to get slot info from token");
    }
}

PHP_METHOD(Module, C_GetTokenInfo) {
    CK_RV rv;
    zend_long slotId;
    zval *pInfo;
    zval retval;

    ZEND_PARSE_PARAMETERS_START(2, 2)
        Z_PARAM_LONG(slotId)
        Z_PARAM_ZVAL(pInfo)
    ZEND_PARSE_PARAMETERS_END();

    pkcs11_object *objval = Z_PKCS11_P(ZEND_THIS);

    if (!objval->initialised) {
        zend_throw_exception(zend_ce_exception, "Uninitialised PKCS11 module", 0);
        return;
    }

    rv = php_C_GetTokenInfo(objval, slotId, &retval);

    ZEND_TRY_ASSIGN_REF_VALUE(pInfo, &retval);

    RETURN_LONG(rv);
}


CK_RV php_C_GetMechanismList(pkcs11_object *objval, zend_long slotId, zval *retval) {

    CK_RV rv;

    CK_ULONG ulMechanismCount;
    rv = objval->functionList->C_GetMechanismList((CK_SLOT_ID)slotId, NULL_PTR, &ulMechanismCount);
    if (rv != CKR_OK) {
        return rv;
    }

    CK_MECHANISM_TYPE_PTR pMechanismList = (CK_MECHANISM_TYPE_PTR) ecalloc(ulMechanismCount, sizeof(CK_MECHANISM_TYPE));
    rv = objval->functionList->C_GetMechanismList((CK_SLOT_ID)slotId, pMechanismList, &ulMechanismCount);
    if (rv != CKR_OK) {
        efree(pMechanismList);
        return rv;
    }

    CK_SLOT_ID i;
    array_init(retval);
    for (i=0; i<ulMechanismCount; i++) {
        add_next_index_long(retval, pMechanismList[i]);
    }
    efree(pMechanismList);

    return rv;
}

PHP_METHOD(Module, getMechanismList) {

    zend_long slotId;

    ZEND_PARSE_PARAMETERS_START(1, 1)
        Z_PARAM_LONG(slotId)
    ZEND_PARSE_PARAMETERS_END();


    pkcs11_object *objval = Z_PKCS11_P(ZEND_THIS);

    if (!objval->initialised) {
        zend_throw_exception(zend_ce_exception, "Uninitialised PKCS11 module", 0);
        return;
    }

    CK_RV rv = php_C_GetMechanismList(objval, slotId, return_value);
    if (rv != CKR_OK) {
        pkcs11_error(rv, "Unable to get mechanism list from token");
    }
}

PHP_METHOD(Module, C_GetMechanismList) {
    CK_RV rv;
    zend_long slotId;
    zval *pMechanismList;
    zval retval;

    ZEND_PARSE_PARAMETERS_START(2, 2)
        Z_PARAM_LONG(slotId)
        Z_PARAM_ZVAL(pMechanismList)
    ZEND_PARSE_PARAMETERS_END();

    pkcs11_object *objval = Z_PKCS11_P(ZEND_THIS);

    if (!objval->initialised) {
        zend_throw_exception(zend_ce_exception, "Uninitialised PKCS11 module", 0);
        return;
    }

    rv = php_C_GetMechanismList(objval, slotId, &retval);

    ZEND_TRY_ASSIGN_REF_VALUE(pMechanismList, &retval);

    RETURN_LONG(rv);
}


CK_RV php_C_GetMechanismInfo(pkcs11_object *objval, zend_long slotId, zend_long mechanismId, zval *retval) {

    CK_MECHANISM_INFO mechanismInfo = {};

    CK_RV rv = objval->functionList->C_GetMechanismInfo((CK_SLOT_ID)slotId, (CK_MECHANISM_TYPE)mechanismId, &mechanismInfo);
    if (rv != CKR_OK) {
        return rv;
    }

    array_init(retval);
    add_assoc_long(retval, "ulMinKeySize", mechanismInfo.ulMinKeySize);
    add_assoc_long(retval, "ulMaxKeySize", mechanismInfo.ulMaxKeySize);
    add_assoc_long(retval, "flags", mechanismInfo.flags);

    return rv;
}

PHP_METHOD(Module, getMechanismInfo) {

    zend_long slotId;
    zend_long mechanismId;

    ZEND_PARSE_PARAMETERS_START(2, 2)
        Z_PARAM_LONG(slotId)
        Z_PARAM_LONG(mechanismId)
    ZEND_PARSE_PARAMETERS_END();

    pkcs11_object *objval = Z_PKCS11_P(ZEND_THIS);

    if (!objval->initialised) {
        zend_throw_exception(zend_ce_exception, "Uninitialised PKCS11 module", 0);
        return;
    }

    CK_RV rv = php_C_GetMechanismInfo(objval, slotId, mechanismId, return_value);
    if (rv != CKR_OK) {
        pkcs11_error(rv, "Unable to get mechanism info");
    }
}

PHP_METHOD(Module, C_GetMechanismInfo) {
    CK_RV rv;
    zend_long slotId;
    zend_long type;
    zval *pInfo;
    zval retval;

    ZEND_PARSE_PARAMETERS_START(3, 3)
        Z_PARAM_LONG(slotId)
        Z_PARAM_LONG(type)
        Z_PARAM_ZVAL(pInfo)
    ZEND_PARSE_PARAMETERS_END();

    pkcs11_object *objval = Z_PKCS11_P(ZEND_THIS);

    if (!objval->initialised) {
        zend_throw_exception(zend_ce_exception, "Uninitialised PKCS11 module", 0);
        return;
    }

    rv = php_C_GetMechanismInfo(objval, slotId, type, &retval);

    ZEND_TRY_ASSIGN_REF_VALUE(pInfo, &retval);

    RETURN_LONG(rv);
}


CK_RV php_C_InitToken(pkcs11_object *objval, zend_long slotId, zend_string *label, zend_string *sopin, zval *retval) {

    CK_MECHANISM_INFO mechanismInfo;
    CK_RV rv = objval->functionList->C_InitToken((CK_SLOT_ID)slotId, (CK_UTF8CHAR_PTR)sopin, ZSTR_LEN(sopin), (CK_UTF8CHAR_PTR)label);
    if (rv != CKR_OK) {
        return rv;
    }

    return rv;
}

PHP_METHOD(Module, initToken) {
    zend_string    *label_str;
    zend_string    *sopin_str;
    zend_long      slotid;

    ZEND_PARSE_PARAMETERS_START(3, 3)
        Z_PARAM_LONG(slotid)
        Z_PARAM_STR(label_str)
        Z_PARAM_STR(sopin_str)
    ZEND_PARSE_PARAMETERS_END();

    pkcs11_object *objval = Z_PKCS11_P(ZEND_THIS);

    if (!objval->initialised) {
        zend_throw_exception(zend_ce_exception, "Uninitialised PKCS11 module", 0);
        return;
    }
    
    CK_RV rv = php_C_InitToken(objval, slotid, label_str, sopin_str, return_value);
    if (rv != CKR_OK) {
        pkcs11_error(rv, "Unable to initialise token");
    }
}

PHP_METHOD(Module, C_InitToken) {
    zend_string    *label_str;
    zend_string    *sopin_str;
    zend_long      slotid;

    ZEND_PARSE_PARAMETERS_START(3, 3)
        Z_PARAM_LONG(slotid)
        Z_PARAM_STR(label_str)
        Z_PARAM_STR(sopin_str)
    ZEND_PARSE_PARAMETERS_END();

    pkcs11_object *objval = Z_PKCS11_P(ZEND_THIS);

    if (!objval->initialised) {
        zend_throw_exception(zend_ce_exception, "Uninitialised PKCS11 module", 0);
        return;
    }

    CK_RV rv = php_C_InitToken(objval, slotid, label_str, sopin_str, return_value);

    RETURN_LONG(rv);
}

PHP_METHOD(Module, openSession) {
    CK_RV rv;

    zend_long      slotid;
    zend_long      flags;
    zend_string    *application;
    zend_fcall_info php_fciNotify;
    zend_fcall_info_cache fciNotify_cache;

    ZEND_PARSE_PARAMETERS_START(1, 4)
        Z_PARAM_LONG(slotid)
        Z_PARAM_OPTIONAL
        Z_PARAM_LONG(flags)
        Z_PARAM_STR(application)
        Z_PARAM_FUNC(php_fciNotify, fciNotify_cache)
    ZEND_PARSE_PARAMETERS_END();

    pkcs11_object *objval = Z_PKCS11_P(ZEND_THIS);

    if (!objval->initialised) {
        zend_throw_exception(zend_ce_exception, "Uninitialised PKCS11 module", 0);
        return;
    }

    pkcs11_session_object* session_obj;

    object_init_ex(return_value, ce_Pkcs11_Session);
    session_obj = Z_PKCS11_SESSION_P(return_value);
    session_obj->pkcs11 = objval;

    CK_SESSION_HANDLE phSession;
    rv = objval->functionList->C_OpenSession(slotid, CKF_SERIAL_SESSION | flags, NULL_PTR, NULL_PTR, &phSession);
    if (rv != CKR_OK) {
        pkcs11_error(rv, "Unable to open session");
        return;
    }
    session_obj->session = phSession;
    session_obj->slotID = slotid;
}

PHP_METHOD(Module, waitForSlotEvent) {
    CK_RV rv;
    CK_SLOT_ID slotID;

    zend_long php_flags;

    ZEND_PARSE_PARAMETERS_START(1, 1)
        Z_PARAM_LONG(php_flags)
    ZEND_PARSE_PARAMETERS_END();

    pkcs11_object *objval = Z_PKCS11_P(ZEND_THIS);

    rv = objval->functionList->C_WaitForSlotEvent((CK_FLAGS)php_flags, &slotID, NULL_PTR);

    if (rv == CKR_OK) {
        RETURN_LONG(slotID);
        return;
    }

    if (rv == CKR_NO_EVENT) {
        RETURN_NULL();
    }

    zend_throw_exception(zend_ce_exception, "Error waiting for events", 0);
    return ;
}

/*
 * fabric (__construct like) of a \Session thru a ref parameter
 */
PHP_METHOD(Module, C_OpenSession) {
    CK_RV rv;

    zend_long      php_slotID;
    zend_long      php_flags;
    zend_string    *php_pApplication = NULL;
    zend_fcall_info php_fciNotify;
    zend_fcall_info_cache fciNotify_cache;
    zval           *php_hSession;

    ZEND_PARSE_PARAMETERS_START(5, 5)
        Z_PARAM_LONG(php_slotID)
        Z_PARAM_LONG(php_flags)
    	Z_PARAM_STR_EX(php_pApplication, 1, 0)
    	Z_PARAM_FUNC_EX(php_fciNotify, fciNotify_cache, 1, 0)
    	Z_PARAM_ZVAL(php_hSession)
    ZEND_PARSE_PARAMETERS_END();
	//Z_PARAM_OBJECT_EX(php_hSession, 1, 0)
	//Z_PARAM_OBJECT_OF_CLASS_EX(php_hSession, ce_Pkcs11_Session, 1, 0)

    pkcs11_object *objval = Z_PKCS11_P(ZEND_THIS);

    if (((CK_FLAGS)php_flags) &
         (CKF_RW_SESSION || CKF_SERIAL_SESSION)) {
        ; /* nope */
    } else {
        ; /* nope */
    }

    pkcs11_session_object* session_objval;

    zval zvhSession = {};
    object_init_ex(&zvhSession, ce_Pkcs11_Session);
    session_objval = Z_PKCS11_SESSION_P(&zvhSession);
    session_objval->pkcs11 = objval;
    GC_ADDREF(&objval->std); /* session is refering the pkcs11 module */

    CK_SESSION_HANDLE hSession = 0;
    if (ZEND_NUM_ARGS() > 4)
        rv = objval->functionList->C_OpenSession((CK_SLOT_ID)php_slotID, php_flags, NULL_PTR, NULL_PTR, &hSession); /* TODO: add callbacks */
    else
        rv = objval->functionList->C_OpenSession((CK_SLOT_ID)php_slotID, php_flags, NULL_PTR, NULL_PTR, &hSession); /* TODO: add callbacks */
    session_objval->session = hSession;
    if (rv != CKR_OK) {
        RETURN_LONG(rv);
    }

    session_objval->slotID = php_slotID;

    ZEND_TRY_ASSIGN_REF_VALUE(php_hSession, &zvhSession);

    RETURN_LONG(rv);
}

ZEND_BEGIN_ARG_INFO_EX(arginfo_C_CloseSession, 0, 0, 1)
    ZEND_ARG_OBJ_INFO(0, session, Pkcs11\\Session, 0)
ZEND_END_ARG_INFO()

PHP_METHOD(Module, C_CloseSession) {
    CK_RV rv;

    zval *php_session;

    ZEND_PARSE_PARAMETERS_START(1, 1)
        Z_PARAM_OBJECT_OF_CLASS(php_session, ce_Pkcs11_Session)
    ZEND_PARSE_PARAMETERS_END();

    pkcs11_object *objval = Z_PKCS11_P(ZEND_THIS);
    pkcs11_session_object *sessionobjval = Z_PKCS11_SESSION_P(php_session);

    rv = sessionobjval->pkcs11->functionList->C_CloseSession(sessionobjval->session);
    // TBC GC_DELREF(&objval->std); /* session is refering the pkcs11 module */
    sessionobjval->session = 0;

    RETURN_LONG(rv);
}

ZEND_BEGIN_ARG_INFO_EX(arginfo_C_GetSessionInfo, 0, 0, 1)
    ZEND_ARG_OBJ_INFO(0, session, Pkcs11\\Session, 0)
    ZEND_ARG_TYPE_INFO(1, pInfo, IS_ARRAY, 1)
ZEND_END_ARG_INFO()

PHP_METHOD(Module, C_GetSessionInfo) {
    CK_RV rv;

    zval *session;
    zval *pInfo;
    zval retval;

    ZEND_PARSE_PARAMETERS_START(2, 2)
        Z_PARAM_OBJECT_OF_CLASS(session, ce_Pkcs11_Session)
        Z_PARAM_ZVAL(pInfo)
    ZEND_PARSE_PARAMETERS_END();

    pkcs11_session_object *sessionobjval = Z_PKCS11_SESSION_P(session);

    rv = php_C_GetSessionInfo(sessionobjval, &retval);

    ZEND_TRY_ASSIGN_REF_VALUE(pInfo, &retval);

    RETURN_LONG(rv);
}

ZEND_BEGIN_ARG_INFO_EX(arginfo_C_GenerateRandom, 0, 0, 1)
    ZEND_ARG_OBJ_INFO(0, session, Pkcs11\\Session, 0)
    ZEND_ARG_TYPE_INFO(0, RandomLen, IS_LONG, 0)
    ZEND_ARG_TYPE_INFO(1, pRandomData, IS_STRING, 1)
ZEND_END_ARG_INFO()

PHP_METHOD(Module, C_GenerateRandom) {
    CK_RV rv;

    zval *php_session;
    zend_long php_RandomLen = 0;
    zval *pRandomData;
    zval retval;

    ZEND_PARSE_PARAMETERS_START(3, 3)
        Z_PARAM_OBJECT_OF_CLASS(php_session, ce_Pkcs11_Session)
        Z_PARAM_LONG(php_RandomLen)
        Z_PARAM_ZVAL(pRandomData)
    ZEND_PARSE_PARAMETERS_END();

    pkcs11_session_object *objval = Z_PKCS11_SESSION_P(php_session);
    rv = php_C_GenerateRandom(objval, php_RandomLen, &retval);

    ZEND_TRY_ASSIGN_REF_VALUE(pRandomData, &retval);

    RETURN_LONG(rv);
}

ZEND_BEGIN_ARG_INFO_EX(arginfo_C_SeedRandom, 0, 0, 1)
    ZEND_ARG_OBJ_INFO(0, session, Pkcs11\\Session, 0)
    ZEND_ARG_TYPE_INFO(0, Seed, IS_STRING, 0)
ZEND_END_ARG_INFO()

PHP_METHOD(Module, C_SeedRandom) {
    CK_RV rv;

    zval *php_session;
    zend_string *php_pSeed = NULL;
    zval retval;

    ZEND_PARSE_PARAMETERS_START(2, 2)
        Z_PARAM_OBJECT_OF_CLASS(php_session, ce_Pkcs11_Session)
        Z_PARAM_STR(php_pSeed)
    ZEND_PARSE_PARAMETERS_END();

    pkcs11_session_object *objval = Z_PKCS11_SESSION_P(php_session);
    rv = php_C_SeedRandom(objval, php_pSeed);

    RETURN_LONG(rv);
}

ZEND_BEGIN_ARG_INFO_EX(arginfo_C_Login, 0, 0, 3)
    ZEND_ARG_OBJ_INFO(0, session, Pkcs11\\Session, 0)
    ZEND_ARG_TYPE_INFO(0, loginType, IS_LONG, 0)
    ZEND_ARG_TYPE_INFO(0, pin, IS_STRING, 0)
ZEND_END_ARG_INFO()

PHP_METHOD(Module, C_Login) {
    CK_RV rv;

    zval *session;
    zend_long userType;
    zend_string *pin;

    ZEND_PARSE_PARAMETERS_START(3, 3)
        Z_PARAM_OBJECT_OF_CLASS(session, ce_Pkcs11_Session)
        Z_PARAM_LONG(userType)
        Z_PARAM_STR(pin)
    ZEND_PARSE_PARAMETERS_END();

    pkcs11_object *objval = Z_PKCS11_P(ZEND_THIS);
    pkcs11_session_object *sessionobjval = Z_PKCS11_SESSION_P(session);

    rv = objval->functionList->C_Login(sessionobjval->session, userType, ZSTR_VAL(pin), ZSTR_LEN(pin));

    RETURN_LONG(rv);
}

ZEND_BEGIN_ARG_INFO_EX(arginfo_C_Logout, 0, 0, 1)
    ZEND_ARG_OBJ_INFO(0, session, Pkcs11\\Session, 0)
ZEND_END_ARG_INFO()

PHP_METHOD(Module, C_Logout) {
    CK_RV rv;

    zval *session;

    ZEND_PARSE_PARAMETERS_START(1, 1)
        Z_PARAM_OBJECT_OF_CLASS(session, ce_Pkcs11_Session)
    ZEND_PARSE_PARAMETERS_END();

    pkcs11_object *objval = Z_PKCS11_P(ZEND_THIS);
    pkcs11_session_object *sessionobjval = Z_PKCS11_SESSION_P(session);

    rv = objval->functionList->C_Logout(sessionobjval->session);

    RETURN_LONG(rv);
}

PHP_METHOD(Module, C_WaitForSlotEvent) {
    CK_RV rv;
    CK_SLOT_ID slotID;

    zend_long php_flags;
    zval *php_slotID = NULL;

    ZEND_PARSE_PARAMETERS_START(2, 2)
        Z_PARAM_LONG(php_flags)
        Z_PARAM_ZVAL(php_slotID)
    ZEND_PARSE_PARAMETERS_END();

    pkcs11_object *objval = Z_PKCS11_P(ZEND_THIS);

    rv = objval->functionList->C_WaitForSlotEvent((CK_FLAGS)php_flags, &slotID, NULL_PTR);

    if (rv == CKR_OK) {
        zval zva;
        ZVAL_LONG(&zva, slotID);
        ZEND_TRY_ASSIGN_REF_VALUE(php_slotID, &zva);
    }

    RETURN_LONG(rv);
}

ZEND_BEGIN_ARG_INFO_EX(arginfo_C_SetPIN, 0, 0, 3)
    ZEND_ARG_OBJ_INFO(0, session, Pkcs11\\Session, 0)
    ZEND_ARG_TYPE_INFO(0, oldPin, IS_STRING, 0)
    ZEND_ARG_TYPE_INFO(0, newPin, IS_STRING, 0)
ZEND_END_ARG_INFO()

PHP_METHOD(Module, C_SetPIN) {
    CK_RV rv;

    zval *session;
    zend_string *oldPin;
    zend_string *newPin;

    ZEND_PARSE_PARAMETERS_START(3, 3)
        Z_PARAM_OBJECT_OF_CLASS(session, ce_Pkcs11_Session)
        Z_PARAM_STR(oldPin)
        Z_PARAM_STR(newPin)
    ZEND_PARSE_PARAMETERS_END();

    pkcs11_object *objval = Z_PKCS11_P(ZEND_THIS);
    pkcs11_session_object *sessionobjval = Z_PKCS11_SESSION_P(session);

    rv = objval->functionList->C_SetPIN(
        sessionobjval->session,
        ZSTR_VAL(oldPin),
        ZSTR_LEN(oldPin),
        ZSTR_VAL(newPin),
        ZSTR_LEN(newPin)
    );

    RETURN_LONG(rv);
}

ZEND_BEGIN_ARG_INFO_EX(arginfo_C_InitPIN, 0, 0, 2)
    ZEND_ARG_OBJ_INFO(0, session, Pkcs11\\Session, 0)
    ZEND_ARG_TYPE_INFO(0, newPin, IS_STRING, 0)
ZEND_END_ARG_INFO()

PHP_METHOD(Module, C_InitPIN) {
    CK_RV rv;

    zval *session;
    zend_string *newPin;

    ZEND_PARSE_PARAMETERS_START(2, 2)
        Z_PARAM_OBJECT_OF_CLASS(session, ce_Pkcs11_Session)
        Z_PARAM_STR(newPin)
    ZEND_PARSE_PARAMETERS_END();

    pkcs11_object *objval = Z_PKCS11_P(ZEND_THIS);
    pkcs11_session_object *sessionobjval = Z_PKCS11_SESSION_P(session);

    rv = objval->functionList->C_InitPIN(
        sessionobjval->session,
        ZSTR_VAL(newPin),
        ZSTR_LEN(newPin)
    );

    RETURN_LONG(rv);
}

ZEND_BEGIN_ARG_INFO_EX(arginfo_C_GenerateKey, 0, 0, 3)
    ZEND_ARG_OBJ_INFO(0, session, Pkcs11\\Session, 0)
    ZEND_ARG_OBJ_INFO(0, mechanism, Pkcs11\\Mechanism, 0)
    ZEND_ARG_TYPE_INFO(0, template, IS_ARRAY, 0)
    ZEND_ARG_OBJ_INFO(1, phKey, Pkcs11\\Key, 1)
ZEND_END_ARG_INFO()

PHP_METHOD(Module, C_GenerateKey) {
    CK_RV rv;

    zval *session;
    zval *mechanism;
    HashTable *template;
    zval *phKey;
    zval retval;

    ZEND_PARSE_PARAMETERS_START(4, 4)
        Z_PARAM_ZVAL(session)
        Z_PARAM_ZVAL(mechanism)
        Z_PARAM_ARRAY_HT(template)
        Z_PARAM_ZVAL(phKey)
    ZEND_PARSE_PARAMETERS_END();

    pkcs11_object *objval = Z_PKCS11_P(ZEND_THIS);
    pkcs11_session_object *sessionobjval = Z_PKCS11_SESSION_P(session);

    rv = php_C_GenerateKey(sessionobjval, mechanism, template, &retval);

    ZEND_TRY_ASSIGN_REF_VALUE(phKey, &retval);

    RETURN_LONG(rv);
}

ZEND_BEGIN_ARG_INFO_EX(arginfo_C_GenerateKeyPair, 0, 0, 4)
    ZEND_ARG_OBJ_INFO(0, session, Pkcs11\\Session, 0)
    ZEND_ARG_OBJ_INFO(0, mechanism, Pkcs11\\Mechanism, 0)
    ZEND_ARG_TYPE_INFO(0, pkTemplate, IS_ARRAY, 0)
    ZEND_ARG_TYPE_INFO(0, skTemplate, IS_ARRAY, 0)
    ZEND_ARG_OBJ_INFO(1, phPublicKey, Pkcs11\\Key, 1)
    ZEND_ARG_OBJ_INFO(1, phPrivateKey, Pkcs11\\Key, 1)
ZEND_END_ARG_INFO()

PHP_METHOD(Module, C_GenerateKeyPair) {
    CK_RV rv;

    zval *session;
    zval *mechanism;
    HashTable *pkTemplate;
    HashTable *skTemplate;
    zval *phPublicKey;
    zval *phPrivateKey;
    zval retvalpk;
    zval retvalsk;

    ZEND_PARSE_PARAMETERS_START(6, 6)
        Z_PARAM_ZVAL(session)
        Z_PARAM_ZVAL(mechanism)
        Z_PARAM_ARRAY_HT(pkTemplate)
        Z_PARAM_ARRAY_HT(skTemplate)
        Z_PARAM_ZVAL(phPublicKey)
        Z_PARAM_ZVAL(phPrivateKey)
    ZEND_PARSE_PARAMETERS_END();
  
    pkcs11_object *objval = Z_PKCS11_P(ZEND_THIS);
    pkcs11_session_object *sessionobjval = Z_PKCS11_SESSION_P(session);

    rv = php_C_GenerateKeyPair(sessionobjval, mechanism, pkTemplate, skTemplate, &retvalpk, &retvalsk);
    ZEND_TRY_ASSIGN_REF_VALUE(phPublicKey, &retvalpk);
    ZEND_TRY_ASSIGN_REF_VALUE(phPrivateKey, &retvalsk);

    RETURN_LONG(rv);
}

ZEND_BEGIN_ARG_INFO_EX(arginfo_C_DigestInit, 0, 0, 2)
    ZEND_ARG_OBJ_INFO(0, session, Pkcs11\\Session, 0)
    ZEND_ARG_OBJ_INFO(0, mechanism, Pkcs11\\Mechanism, 0)
ZEND_END_ARG_INFO()

PHP_METHOD(Module, C_DigestInit) {
    CK_RV rv;

    zval *session;
    zval *mechanism;

    ZEND_PARSE_PARAMETERS_START(2, 2)
        Z_PARAM_ZVAL(session)
        Z_PARAM_ZVAL(mechanism)
    ZEND_PARSE_PARAMETERS_END();

    pkcs11_object *objval = Z_PKCS11_P(ZEND_THIS);
    pkcs11_session_object *sessionobjval = Z_PKCS11_SESSION_P(session);

    pkcs11_mechanism_object *mechanismObjval = Z_PKCS11_MECHANISM_P(mechanism);

    rv = sessionobjval->pkcs11->functionList->C_DigestInit(
        sessionobjval->session,
        &mechanismObjval->mechanism
    );
    if (rv != CKR_OK) {
        pkcs11_error(rv, "Unable to initialize digest");
        return;
    }
}


ZEND_BEGIN_ARG_INFO_EX(arginfo_C_Digest, 0, 0, 2)
    ZEND_ARG_OBJ_INFO(0, session, Pkcs11\\Session, 0)
    ZEND_ARG_TYPE_INFO(0, data, IS_STRING, 0)
ZEND_END_ARG_INFO()

PHP_METHOD(Module, C_Digest) {
    CK_RV rv;

    zval *session;
    zend_string *data;

    ZEND_PARSE_PARAMETERS_START(2, 2)
        Z_PARAM_ZVAL(session)
        Z_PARAM_STR(data)
    ZEND_PARSE_PARAMETERS_END();

    pkcs11_object *objval = Z_PKCS11_P(ZEND_THIS);
    pkcs11_session_object *sessionobjval = Z_PKCS11_SESSION_P(session);

    CK_ULONG digestLen;
    rv = sessionobjval->pkcs11->functionList->C_Digest(
        sessionobjval->session,
        ZSTR_VAL(data),
        ZSTR_LEN(data),
        NULL_PTR,
        &digestLen
    );
    if (rv != CKR_OK) {
        pkcs11_error(rv, "Unable to digest");
        return;
    }

    CK_BYTE_PTR digest = ecalloc(digestLen, sizeof(CK_BYTE));
    rv = sessionobjval->pkcs11->functionList->C_Digest(
        sessionobjval->session,
        ZSTR_VAL(data),
        ZSTR_LEN(data),
        digest,
        &digestLen
    );
    if (rv != CKR_OK) {
        pkcs11_error(rv, "Unable to digest");
        return;
    }

    zend_string *returnval;
    returnval = zend_string_alloc(digestLen, 0);
    memcpy(
        ZSTR_VAL(returnval),
        digest,
        digestLen
    );
    RETURN_STR(returnval);
 
    efree(digest);
}

ZEND_BEGIN_ARG_INFO_EX(arginfo_C_DigestUpdate, 0, 0, 2)
    ZEND_ARG_OBJ_INFO(0, session, Pkcs11\\Session, 0)
    ZEND_ARG_TYPE_INFO(0, part, IS_STRING, 0)
ZEND_END_ARG_INFO()

PHP_METHOD(Module, C_DigestUpdate) {
    CK_RV rv;

    zval *session;
    zend_string *part;

    ZEND_PARSE_PARAMETERS_START(2, 2)
        Z_PARAM_ZVAL(session)
        Z_PARAM_STR(part)
    ZEND_PARSE_PARAMETERS_END();

    pkcs11_object *objval = Z_PKCS11_P(ZEND_THIS);
    pkcs11_session_object *sessionobjval = Z_PKCS11_SESSION_P(session);

    rv = sessionobjval->pkcs11->functionList->C_DigestUpdate(
        sessionobjval->session,
        ZSTR_VAL(part),
        ZSTR_LEN(part)
    );
    if (rv != CKR_OK) {
        pkcs11_error(rv, "Unable to update digest");
        return;
    }
}

ZEND_BEGIN_ARG_INFO_EX(arginfo_C_DigestKey, 0, 0, 2)
    ZEND_ARG_OBJ_INFO(0, session, Pkcs11\\Session, 0)
    ZEND_ARG_OBJ_INFO(0, key, Pkcs11\\Key, 0)
ZEND_END_ARG_INFO()

PHP_METHOD(Module, C_DigestKey) {
    CK_RV rv;

    zval *session;
    zval *key;

    ZEND_PARSE_PARAMETERS_START(2, 2)
        Z_PARAM_ZVAL(session)
        Z_PARAM_ZVAL(key)
    ZEND_PARSE_PARAMETERS_END();

    pkcs11_object *objval = Z_PKCS11_P(ZEND_THIS);
    pkcs11_session_object *sessionobjval = Z_PKCS11_SESSION_P(session);
    pkcs11_key_object *keyobjval = Z_PKCS11_KEY_P(key);

    rv = sessionobjval->pkcs11->functionList->C_DigestKey(
        sessionobjval->session,
        keyobjval->key
    );
    if (rv != CKR_OK) {
        pkcs11_error(rv, "Unable to update digest");
        return;
    }
}

ZEND_BEGIN_ARG_INFO_EX(arginfo_C_DigestFinal, 0, 0, 1)
    ZEND_ARG_OBJ_INFO(0, session, Pkcs11\\Session, 0)
ZEND_END_ARG_INFO()

PHP_METHOD(Module, C_DigestFinal) {
    CK_RV rv;

    zval *session;

    ZEND_PARSE_PARAMETERS_START(1, 1)
        Z_PARAM_ZVAL(session)
    ZEND_PARSE_PARAMETERS_END();

    pkcs11_object *objval = Z_PKCS11_P(ZEND_THIS);
    pkcs11_session_object *sessionobjval = Z_PKCS11_SESSION_P(session);

    CK_ULONG digestLen;
    rv = sessionobjval->pkcs11->functionList->C_DigestFinal(
        sessionobjval->session,
        NULL_PTR,
        &digestLen
    );
    if (rv != CKR_OK) {
        pkcs11_error(rv, "Unable to finalize digest");
        return;
    }

    CK_BYTE_PTR digest = ecalloc(digestLen, sizeof(CK_BYTE));
    rv = sessionobjval->pkcs11->functionList->C_DigestFinal(
        sessionobjval->session,
        digest,
        &digestLen
    );
    if (rv != CKR_OK) {
        pkcs11_error(rv, "Unable to finalize digest");
        return;
    }

    zend_string *returnval;
    returnval = zend_string_alloc(digestLen, 0);
    memcpy(
        ZSTR_VAL(returnval),
        digest,
        digestLen
    );
    RETURN_STR(returnval);
 
    efree(digest);
}

ZEND_BEGIN_ARG_INFO_EX(arginfo_C_SignInit, 0, 0, 3)
    ZEND_ARG_OBJ_INFO(0, session, Pkcs11\\Session, 0)
    ZEND_ARG_OBJ_INFO(0, mechanism, Pkcs11\\Mechanism, 0)
    ZEND_ARG_TYPE_INFO(0, key, IS_LONG, 0)
ZEND_END_ARG_INFO()

PHP_METHOD(Module, C_SignInit) {
    CK_RV rv;
    CK_OBJECT_HANDLE hKey;

    zval *mechanism;
    zend_long key;
    zval *session;

    ZEND_PARSE_PARAMETERS_START(3, 3)
        Z_PARAM_OBJECT_OF_CLASS(session, ce_Pkcs11_Session)
        Z_PARAM_OBJECT_OF_CLASS(mechanism, ce_Pkcs11_Mechanism)
        Z_PARAM_LONG(key)
    ZEND_PARSE_PARAMETERS_END();

    pkcs11_mechanism_object * const oMechanism = Z_PKCS11_MECHANISM_P(mechanism);

    if (oMechanism->mechanism.mechanism == 0) {
        zend_throw_exception(zend_ce_exception, "Invalid mechanism", 0);
        return ;
    }

    pkcs11_object *objval = Z_PKCS11_P(ZEND_THIS);
    pkcs11_session_object *sessionobjval = Z_PKCS11_SESSION_P(session);

    hKey = (CK_OBJECT_HANDLE)key;

    rv = objval->functionList->C_SignInit(sessionobjval->session, &oMechanism->mechanism, hKey);

    RETURN_LONG(rv);
}

ZEND_BEGIN_ARG_INFO_EX(arginfo_C_Sign, 0, 0, 3)
    ZEND_ARG_OBJ_INFO(0, session, Pkcs11\\Session, 0)
    ZEND_ARG_TYPE_INFO(0, data, IS_STRING, 0)
    ZEND_ARG_TYPE_INFO(1, signature, IS_STRING, 1)
ZEND_END_ARG_INFO()

PHP_METHOD(Module, C_Sign) {
    CK_RV rv;
    CK_BYTE_PTR pData;
    CK_ULONG ulDataLen;
    CK_BYTE_PTR pSignature = NULL;
    CK_ULONG ulSignatureLen = 0;

    zval *session;
    zend_string *data = NULL;
    zval *signature = NULL;

    ZEND_PARSE_PARAMETERS_START(3, 3)
        Z_PARAM_OBJECT_OF_CLASS(session, ce_Pkcs11_Session)
        Z_PARAM_STR(data)
        Z_PARAM_ZVAL(signature)
    ZEND_PARSE_PARAMETERS_END();

    pkcs11_object *objval = Z_PKCS11_P(ZEND_THIS);
    pkcs11_session_object *sessionobjval = Z_PKCS11_SESSION_P(session);

    pData = (CK_BYTE_PTR)ZSTR_VAL(data);
    ulDataLen = (CK_ULONG)ZSTR_LEN(data);

    /* first, get the length of the Signature */
    rv = objval->functionList->C_Sign(sessionobjval->session, pData, ulDataLen, NULL, &ulSignatureLen);
    if (rv != CKR_OK) {
        RETURN_LONG(rv);
        return ;
    }

    pSignature = ecalloc(sizeof(*pSignature), ulSignatureLen);

    rv = objval->functionList->C_Sign(sessionobjval->session, pData, ulDataLen, pSignature, &ulSignatureLen);

    zval retval;
    ZVAL_STRINGL(&retval, (char *)pSignature, ulSignatureLen);
    efree(pSignature);

    ZEND_TRY_ASSIGN_REF_VALUE(signature, &retval);

    RETURN_LONG(rv);
}

ZEND_BEGIN_ARG_INFO_EX(arginfo_C_VerifyInit, 0, 0, 3)
    ZEND_ARG_OBJ_INFO(0, session, Pkcs11\\Session, 0)
    ZEND_ARG_OBJ_INFO(0, mechanism, Pkcs11\\Mechanism, 0)
    ZEND_ARG_TYPE_INFO(0, key, IS_LONG, 0)
ZEND_END_ARG_INFO()

PHP_METHOD(Module, C_VerifyInit) {
    CK_RV rv;
    CK_OBJECT_HANDLE hKey;

    zval *mechanism;
    zend_long key;
    zval *session;

    ZEND_PARSE_PARAMETERS_START(3, 3)
        Z_PARAM_OBJECT_OF_CLASS(session, ce_Pkcs11_Session)
        Z_PARAM_OBJECT_OF_CLASS(mechanism, ce_Pkcs11_Mechanism)
        Z_PARAM_LONG(key)
    ZEND_PARSE_PARAMETERS_END();

    pkcs11_mechanism_object * const oMechanism = Z_PKCS11_MECHANISM_P(mechanism);

    if (oMechanism->mechanism.mechanism == 0) {
        zend_throw_exception(zend_ce_exception, "Invalid mechanism", 0);
        return ;
    }

    pkcs11_object *objval = Z_PKCS11_P(ZEND_THIS);
    pkcs11_session_object *sessionobjval = Z_PKCS11_SESSION_P(session);

    hKey = (CK_OBJECT_HANDLE)key;

    rv = objval->functionList->C_VerifyInit(sessionobjval->session, &oMechanism->mechanism, hKey);

    RETURN_LONG(rv);
}

ZEND_BEGIN_ARG_INFO_EX(arginfo_C_Verify, 0, 0, 3)
    ZEND_ARG_OBJ_INFO(0, session, Pkcs11\\Session, 0)
    ZEND_ARG_TYPE_INFO(0, data, IS_STRING, 0)
    ZEND_ARG_TYPE_INFO(0, signature, IS_STRING, 0)
ZEND_END_ARG_INFO()

PHP_METHOD(Module, C_Verify) {
    CK_RV rv;
    CK_BYTE_PTR pData;
    CK_ULONG ulDataLen;
    CK_BYTE_PTR pSignature = NULL;
    CK_ULONG ulSignatureLen = 0;

    zval *session;
    zend_string *data = NULL;
    zend_string *signature = NULL;

    ZEND_PARSE_PARAMETERS_START(3, 3)
        Z_PARAM_OBJECT_OF_CLASS(session, ce_Pkcs11_Session)
        Z_PARAM_STR(data)
        Z_PARAM_STR(signature)
    ZEND_PARSE_PARAMETERS_END();

    pkcs11_object *objval = Z_PKCS11_P(ZEND_THIS);
    pkcs11_session_object *sessionobjval = Z_PKCS11_SESSION_P(session);

    pData = (CK_BYTE_PTR)ZSTR_VAL(data);
    ulDataLen = (CK_ULONG)ZSTR_LEN(data);
    pSignature = (CK_BYTE_PTR)ZSTR_VAL(signature);
    ulSignatureLen = (CK_ULONG)ZSTR_LEN(signature);

    rv = objval->functionList->C_Verify(sessionobjval->session, pData, ulDataLen, pSignature, ulSignatureLen);

    RETURN_LONG(rv);
}

ZEND_BEGIN_ARG_INFO_EX(arginfo_C_CreateObject, 0, 0, 3)
    ZEND_ARG_OBJ_INFO(0, session, Pkcs11\\Session, 0)
    ZEND_ARG_TYPE_INFO(0, template, IS_ARRAY, 0)
    ZEND_ARG_OBJ_INFO(1, phObject, Pkcs11\\P11Object, 1)
ZEND_END_ARG_INFO()


PHP_METHOD(Module, C_CreateObject) {
    CK_RV rv;

    zval *session;
    zval *phObject;
    HashTable *template;
    zval retval;

    ZEND_PARSE_PARAMETERS_START(3, 3)
        Z_PARAM_OBJECT_OF_CLASS(session, ce_Pkcs11_Session)
        Z_PARAM_ARRAY_HT(template)
        Z_PARAM_ZVAL(phObject)
    ZEND_PARSE_PARAMETERS_END();

    pkcs11_object *objval = Z_PKCS11_P(ZEND_THIS);
    pkcs11_session_object *sessionobjval = Z_PKCS11_SESSION_P(session);

    rv = php_C_CreateObject(sessionobjval, template, &retval);

    ZEND_TRY_ASSIGN_REF_VALUE(phObject, &retval);

    RETURN_LONG(rv);
}

static int AssertAttributeCKA(const CK_ATTRIBUTE_PTR pAttribute) {
    switch(pAttribute->type) {
        case CKA_KEY_TYPE:
            return !(pAttribute->ulValueLen == sizeof(CK_KEY_TYPE));
        case CKA_CLASS:
            return !(pAttribute->ulValueLen == sizeof(CK_OBJECT_CLASS));
        case CKA_CERTIFICATE_TYPE:
            return !(pAttribute->ulValueLen == sizeof(CK_CERTIFICATE_TYPE));
        case CKA_TOKEN:
        case CKA_PRIVATE:
        case CKA_TRUSTED:
        case CKA_SENSITIVE:
        case CKA_ENCRYPT:
        case CKA_DECRYPT:
        case CKA_WRAP:
        case CKA_UNWRAP:
        case CKA_SIGN:
        case CKA_SIGN_RECOVER:
        case CKA_VERIFY:
        case CKA_VERIFY_RECOVER:
        case CKA_DERIVE:
        case CKA_EXTRACTABLE:
        case CKA_LOCAL:
        case CKA_NEVER_EXTRACTABLE:
        case CKA_ALWAYS_SENSITIVE:
        case CKA_ALWAYS_AUTHENTICATE:
        case CKA_WRAP_WITH_TRUSTED:
        case CKA_RESET_ON_INIT:
        case CKA_HAS_RESET:
        case CKA_COLOR:
            return !(pAttribute->ulValueLen == sizeof(CK_BBOOL));
        case CKA_CERTIFICATE_CATEGORY:
        case CKA_JAVA_MIDP_SECURITY_DOMAIN:
        case CKA_MODULUS_BITS:
        case CKA_PRIME_BITS:
        case CKA_SUB_PRIME_BITS:
        case CKA_VALUE_BITS:
        case CKA_VALUE_LEN:
        case CKA_PIXEL_X:
        case CKA_PIXEL_Y:
        case CKA_RESOLUTION:
        case CKA_CHAR_ROWS:
        case CKA_CHAR_COLUMNS:
        case CKA_BITS_PER_PIXEL:
            return !(pAttribute->ulValueLen == sizeof(CK_ULONG));
        case CKA_VALUE:
        case CKA_OBJECT_ID:
        case CKA_SERIAL_NUMBER:
        case CKA_ATTR_TYPES:
        case CKA_HASH_OF_SUBJECT_PUBLIC_KEY:
        case CKA_HASH_OF_ISSUER_PUBLIC_KEY:
        case CKA_CHECK_VALUE:
        case CKA_ECDSA_PARAMS:
        case CKA_LABEL:
        case CKA_EC_POINT:
        case CKA_REQUIRED_CMS_ATTRIBUTES:
        case CKA_DEFAULT_CMS_ATTRIBUTES:
        case CKA_SUPPORTED_CMS_ATTRIBUTES:
        case CKA_ID:
        case CKA_APPLICATION:
        case CKA_URL:
        case CKA_CHAR_SETS:
        case CKA_ENCODING_METHODS:
        case CKA_MIME_TYPES:
        default: /* TBD */
            return 0; /* any length */
    }
    return 0;
}

ZEND_BEGIN_ARG_INFO_EX(arginfo_C_FindObjectsInit, 0, 0, 2)
    ZEND_ARG_OBJ_INFO(0, session, Pkcs11\\Session, 0)
    ZEND_ARG_TYPE_INFO(0, template, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

PHP_METHOD(Module, C_FindObjectsInit) {
    CK_RV rv;

    zval *session;
    HashTable *template = NULL; /* PHP array */

    CK_ULONG ulCount = 0; /* number of attributes in the search template */
    CK_ATTRIBUTE_PTR pTemplate = NULL;

    ZEND_PARSE_PARAMETERS_START(1, 2)
        Z_PARAM_OBJECT_OF_CLASS(session, ce_Pkcs11_Session)
        Z_PARAM_OPTIONAL
        Z_PARAM_ARRAY_HT(template)
    ZEND_PARSE_PARAMETERS_END();

    pkcs11_object *objval = Z_PKCS11_P(ZEND_THIS);
    pkcs11_session_object *sessionobjval = Z_PKCS11_SESSION_P(session);

    if (template)
        parseTemplate(&template, &pTemplate, (int *)&ulCount);

    for(int i = 0; i < ulCount; i++) {
        if (AssertAttributeCKA(&pTemplate[i])) {
            zend_throw_exception(zend_ce_exception, "sizeof('value') invalid for requested type", 0);
            freeTemplate(pTemplate);
            return ;
        }
    }

    rv = objval->functionList->C_FindObjectsInit(sessionobjval->session, pTemplate, ulCount);
    if (pTemplate)
        freeTemplate(pTemplate);

    RETURN_LONG(rv);
}

ZEND_BEGIN_ARG_INFO_EX(arginfo_C_FindObjects, 0, 0, 2)
    ZEND_ARG_OBJ_INFO(0, session, Pkcs11\\Session, 0)
    ZEND_ARG_TYPE_INFO(1, Objects, IS_ARRAY, 1)
    ZEND_ARG_TYPE_INFO(0, MaxObjectCount, IS_LONG, 0) // Default 32
ZEND_END_ARG_INFO()

PHP_METHOD(Module, C_FindObjects) {
    CK_RV rv;
    CK_OBJECT_HANDLE_PTR phObject = NULL;
    CK_ULONG ulMaxObjectCount = 32; /* default is a batch of 32 */
    CK_ULONG ulObjectCount;

    zval *session;
    zend_long MaxObjectCount = ulMaxObjectCount;
    zval *Objects;

    ZEND_PARSE_PARAMETERS_START(2, 3)
        Z_PARAM_OBJECT_OF_CLASS(session, ce_Pkcs11_Session)
        Z_PARAM_ZVAL(Objects)
        Z_PARAM_OPTIONAL
        Z_PARAM_LONG(MaxObjectCount)
    ZEND_PARSE_PARAMETERS_END();

    pkcs11_object *objval = Z_PKCS11_P(ZEND_THIS);
    pkcs11_session_object *sessionobjval = Z_PKCS11_SESSION_P(session);

    if (MaxObjectCount < 1) {
        zend_throw_exception(zend_ce_exception, "Invalid MaxObjectCount argument", 0);
        return ;
    }
    ulMaxObjectCount = (CK_ULONG)MaxObjectCount;

    phObject = (CK_OBJECT_HANDLE_PTR)ecalloc(sizeof(*phObject), ulMaxObjectCount);

    rv = objval->functionList->C_FindObjects(sessionobjval->session,
                                             phObject, ulMaxObjectCount, &ulObjectCount);

    zval O;
    array_init(&O);
    for(CK_ULONG i = 0; i < ulObjectCount; i++)
        add_next_index_long(&O, phObject[i]);

    ZEND_TRY_ASSIGN_REF_VALUE(Objects, &O);

    efree(phObject);

    RETURN_LONG(rv);
}

ZEND_BEGIN_ARG_INFO_EX(arginfo_C_FindObjectsFinal, 0, 0, 1)
    ZEND_ARG_OBJ_INFO(0, session, Pkcs11\\Session, 0)
ZEND_END_ARG_INFO()

PHP_METHOD(Module, C_FindObjectsFinal) {
    CK_RV rv;

    zval *session;

    ZEND_PARSE_PARAMETERS_START(1, 1)
        Z_PARAM_ZVAL(session)
    ZEND_PARSE_PARAMETERS_END();

    pkcs11_object *objval = Z_PKCS11_P(ZEND_THIS);
    pkcs11_session_object *sessionobjval = Z_PKCS11_SESSION_P(session);

    rv = objval->functionList->C_FindObjectsFinal(sessionobjval->session);

    RETURN_LONG(rv);
}

ZEND_BEGIN_ARG_INFO_EX(arginfo_C_GetAttributeValue, 0, 0, 3)
    ZEND_ARG_OBJ_INFO(0, session, Pkcs11\\Session, 0)
    ZEND_ARG_TYPE_INFO(0, object, IS_LONG, 0)
    ZEND_ARG_TYPE_INFO(1, template, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

PHP_METHOD(Module, C_GetAttributeValue) {
    CK_RV rv = 0;
    CK_OBJECT_HANDLE hObject;
    CK_ATTRIBUTE_PTR pTemplate = NULL;
    CK_ULONG ulCount = 0;

    char **infos = NULL;

    zval *session;
    zend_long object;
    HashTable *template = NULL; /* PHP array */

    ZEND_PARSE_PARAMETERS_START(3, 3)
        Z_PARAM_OBJECT_OF_CLASS(session, ce_Pkcs11_Session)
        Z_PARAM_LONG(object)
        Z_PARAM_ARRAY_HT_EX2(template, 0, 1 /* deref*/, 0) // &$template is a reference
    ZEND_PARSE_PARAMETERS_END();

    pkcs11_object *objval = Z_PKCS11_P(ZEND_THIS);
    pkcs11_session_object *sessionobjval = Z_PKCS11_SESSION_P(session);

    hObject = (CK_OBJECT_HANDLE)object;

    parseTemplate(&template, &pTemplate, (int *)&ulCount);

    if (ulCount < 1) {
        zend_throw_exception(zend_ce_exception, "Invalid Template size", 0);
        freeTemplate(pTemplate);
        return ;
    }

    /* first step: fetch the length of each entry */
    rv = objval->functionList->C_GetAttributeValue(sessionobjval->session,
                                                   hObject, pTemplate, ulCount);
    /*
     * Note that the error codes CKR_ATTRIBUTE_SENSITIVE, CKR_ATTRIBUTE_TYPE_INVALID,
     * and CKR_BUFFER_TOO_SMALL do not denote true errors for C_GetAttributeValue.
     * If a call to C_GetAttributeValue returns any of these three values, then the
     * call MUST nonetheless have processed every attribute in the template supplied
     * to C_GetAttributeValue.
     * Each attribute in the template whose value can be returned by the call to
     * C_GetAttributeValue will be returned by the call to C_GetAttributeValue.
     */
    switch(rv) {
        case CKR_ATTRIBUTE_SENSITIVE:
        case CKR_ATTRIBUTE_TYPE_INVALID: /* one of the requested attributed does not exist */
        case CKR_BUFFER_TOO_SMALL:
        case CKR_OK:
            break; /* ok */
        default:
            zend_throw_exception(zend_ce_exception, "error get size C_GetAttributeValue(): ", rv);
            freeTemplate(pTemplate);
            return ;
    }

    for (CK_ULONG k = 0; k < ulCount; k++) {
        if ((pTemplate[k].ulValueLen == CK_UNAVAILABLE_INFORMATION) ||
            (pTemplate[k].ulValueLen < 1))
            continue;
        pTemplate[k].pValue = (CK_BYTE_PTR)ecalloc(1, pTemplate[k].ulValueLen);
    }

    /* fetch the content */
    rv = objval->functionList->C_GetAttributeValue(sessionobjval->session,
                                                   hObject, pTemplate, ulCount);
    /*
     * Note that the error codes CKR_ATTRIBUTE_SENSITIVE, CKR_ATTRIBUTE_TYPE_INVALID,
     * and CKR_BUFFER_TOO_SMALL do not denote true errors for C_GetAttributeValue.
     * If a call to C_GetAttributeValue returns any of these three values, then the
     * call MUST nonetheless have processed every attribute in the template supplied
     * to C_GetAttributeValue.
     * Each attribute in the template whose value can be returned by the call to
     * C_GetAttributeValue will be returned by the call to C_GetAttributeValue.
     */
    switch(rv) {
        case CKR_ATTRIBUTE_SENSITIVE:
        case CKR_ATTRIBUTE_TYPE_INVALID: /* one of the requested attributed does not exist */
        case CKR_BUFFER_TOO_SMALL:
        case CKR_OK:
            break; /* ok */
        default:
            goto fini; /* free pTemplate, pTemplate[x].pValue */
            RETURN_LONG(rv); // placeholder, will be performed into the fini section
    }

    for(CK_ULONG k = 0; k < ulCount; k++) {
        zval zva;
        zval tmp;

        switch(pTemplate[k].ulValueLen) {
          case CK_UNAVAILABLE_INFORMATION:
            zend_hash_index_del(template, pTemplate[k].type);
          break;
          case 0:
            ZVAL_NULL(&tmp);
            zend_hash_index_update(template, pTemplate[k].type, &tmp);
          break;
          default:
            /* add_index_stringl(&template, k, pTemplate[k].pValue, pTemplate[k].ulValueLen); */
            ZVAL_STRINGL(&tmp, pTemplate[k].pValue, pTemplate[k].ulValueLen);
            zend_hash_index_update(template, pTemplate[k].type, &tmp);
          break;
        }
    }

fini: /* memory free section */
    for(CK_ULONG k = 0; k < ulCount; k++) {
        if ((pTemplate[k].ulValueLen == CK_UNAVAILABLE_INFORMATION) ||
            (pTemplate[k].ulValueLen < 1)) {
            continue;
        }
        efree(pTemplate[k].pValue);
    }

    freeTemplate(pTemplate);

    RETURN_LONG(rv);
}

ZEND_BEGIN_ARG_INFO_EX(arginfo_C_CopyObject, 0, 0, 4)
    ZEND_ARG_OBJ_INFO(0, session, Pkcs11\\Session, 0)
    ZEND_ARG_OBJ_INFO(0, object, Pkcs11\\P11Object, 0)
    ZEND_ARG_TYPE_INFO(0, template, IS_ARRAY, 0)
    ZEND_ARG_OBJ_INFO(1, phNewObject, Pkcs11\\P11Object, 1)
ZEND_END_ARG_INFO()

PHP_METHOD(Module, C_CopyObject) {
    CK_RV rv;

    zval *session;
    zval *object;
    zval *phNewObject;
    HashTable *template;
    zval retval;

    ZEND_PARSE_PARAMETERS_START(4, 4)
        Z_PARAM_OBJECT_OF_CLASS(session, ce_Pkcs11_Session)
        Z_PARAM_OBJECT_OF_CLASS(object, ce_Pkcs11_P11Object)
        Z_PARAM_ARRAY_HT(template)
        Z_PARAM_ZVAL(phNewObject)
    ZEND_PARSE_PARAMETERS_END();

    pkcs11_object *objval = Z_PKCS11_P(ZEND_THIS);
    pkcs11_session_object *sessionobjval = Z_PKCS11_SESSION_P(session);

    rv = php_C_CopyObject(sessionobjval, object, template, &retval);

    ZEND_TRY_ASSIGN_REF_VALUE(phNewObject, &retval);

    RETURN_LONG(rv);
}


ZEND_BEGIN_ARG_INFO_EX(arginfo_C_DestroyObject, 0, 0, 2)
    ZEND_ARG_OBJ_INFO(0, session, Pkcs11\\Session, 0)
    ZEND_ARG_OBJ_INFO(0, object, Pkcs11\\P11Object, 0)
ZEND_END_ARG_INFO()

PHP_METHOD(Module, C_DestroyObject) {
    CK_RV rv;

    zval *session;
    zval *object;

    ZEND_PARSE_PARAMETERS_START(2, 2)
        Z_PARAM_ZVAL(session)
        Z_PARAM_ZVAL(object)
    ZEND_PARSE_PARAMETERS_END();

    pkcs11_object *objval = Z_PKCS11_P(ZEND_THIS);
    pkcs11_session_object *sessionobjval = Z_PKCS11_SESSION_P(session);

    rv = php_C_DestroyObject(sessionobjval, object);

    RETURN_LONG(rv);
}

void pkcs11_shutdown(pkcs11_object *obj) {
    // called before the pkcs11_object is freed
    if (obj->functionList != NULL) {
        obj->functionList->C_Finalize(NULL_PTR);
        obj->functionList = NULL;
    }

    if (obj->pkcs11module != NULL) {
        dlclose(obj->pkcs11module);
    }
}


static zend_function_entry module_class_functions[] = {
    PHP_ME(Module, __construct,      arginfo___construct,      ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
    PHP_ME(Module, getInfo,          arginfo_getInfo,          ZEND_ACC_PUBLIC)
    PHP_ME(Module, getSlots,         arginfo_getSlots,         ZEND_ACC_PUBLIC)
    PHP_ME(Module, getSlotList,      arginfo_getSlotList,      ZEND_ACC_PUBLIC)
    PHP_ME(Module, getSlotInfo,      arginfo_getSlotInfo,      ZEND_ACC_PUBLIC)
    PHP_ME(Module, getTokenInfo,     arginfo_getTokenInfo,     ZEND_ACC_PUBLIC)
    PHP_ME(Module, getMechanismList, arginfo_getMechanismList, ZEND_ACC_PUBLIC)
    PHP_ME(Module, getMechanismInfo, arginfo_getMechanismInfo, ZEND_ACC_PUBLIC)
    PHP_ME(Module, initToken,        arginfo_initToken,        ZEND_ACC_PUBLIC)
    PHP_ME(Module, openSession,      arginfo_openSession,      ZEND_ACC_PUBLIC)
    PHP_ME(Module, waitForSlotEvent, arginfo_waitForSlotEvent, ZEND_ACC_PUBLIC)

    PHP_ME(Module, C_GetInfo,          arginfo_C_GetInfo,          ZEND_ACC_PUBLIC)
    PHP_ME(Module, C_GetSlotList,      arginfo_C_GetSlotList,      ZEND_ACC_PUBLIC)
    PHP_ME(Module, C_GetSlotInfo,      arginfo_C_GetSlotInfo,      ZEND_ACC_PUBLIC)
    PHP_ME(Module, C_GetTokenInfo,     arginfo_C_GetTokenInfo,     ZEND_ACC_PUBLIC)
    PHP_ME(Module, C_GetMechanismList, arginfo_C_GetMechanismList, ZEND_ACC_PUBLIC)
    PHP_ME(Module, C_GetMechanismInfo, arginfo_C_GetMechanismInfo, ZEND_ACC_PUBLIC)
    PHP_ME(Module, C_InitToken,        arginfo_C_InitToken,        ZEND_ACC_PUBLIC)
    PHP_ME(Module, C_SetPIN,           arginfo_C_SetPIN,           ZEND_ACC_PUBLIC)
    PHP_ME(Module, C_InitPIN,          arginfo_C_InitPIN,          ZEND_ACC_PUBLIC)
    PHP_ME(Module, C_OpenSession,      arginfo_C_OpenSession,      ZEND_ACC_PUBLIC)
    PHP_ME(Module, C_CloseSession,     arginfo_C_CloseSession,     ZEND_ACC_PUBLIC)
    PHP_ME(Module, C_GetSessionInfo,   arginfo_C_GetSessionInfo,   ZEND_ACC_PUBLIC)
    PHP_ME(Module, C_Login,            arginfo_C_Login,            ZEND_ACC_PUBLIC)
    PHP_ME(Module, C_Logout,           arginfo_C_Logout,           ZEND_ACC_PUBLIC)
    PHP_ME(Module, C_WaitForSlotEvent, arginfo_C_WaitForSlotEvent, ZEND_ACC_PUBLIC)

    PHP_ME(Module, C_GenerateKey,             arginfo_C_GenerateKey,             ZEND_ACC_PUBLIC)
    PHP_ME(Module, C_GenerateKeyPair,         arginfo_C_GenerateKeyPair,         ZEND_ACC_PUBLIC)
    PHP_ME(Module, C_DigestInit,              arginfo_C_DigestInit,              ZEND_ACC_PUBLIC)
    PHP_ME(Module, C_Digest,                  arginfo_C_Digest,                  ZEND_ACC_PUBLIC)
    PHP_ME(Module, C_DigestUpdate,            arginfo_C_DigestUpdate,            ZEND_ACC_PUBLIC)
    PHP_ME(Module, C_DigestKey,               arginfo_C_DigestKey,               ZEND_ACC_PUBLIC)
    PHP_ME(Module, C_DigestFinal,             arginfo_C_DigestFinal,             ZEND_ACC_PUBLIC)

    PHP_ME(Module, C_SignInit,                arginfo_C_SignInit,                ZEND_ACC_PUBLIC)
    PHP_ME(Module, C_Sign,                    arginfo_C_Sign,                    ZEND_ACC_PUBLIC)

    PHP_ME(Module, C_VerifyInit,              arginfo_C_VerifyInit,              ZEND_ACC_PUBLIC)
    PHP_ME(Module, C_Verify,                  arginfo_C_Verify,                  ZEND_ACC_PUBLIC)

    PHP_ME(Module, C_GenerateRandom,          arginfo_C_GenerateRandom,          ZEND_ACC_PUBLIC)
    PHP_ME(Module, C_SeedRandom,              arginfo_C_SeedRandom,              ZEND_ACC_PUBLIC)
    
    PHP_ME(Module, C_CreateObject,            arginfo_C_CreateObject,            ZEND_ACC_PUBLIC)
    PHP_ME(Module, C_FindObjectsInit,         arginfo_C_FindObjectsInit,         ZEND_ACC_PUBLIC)
    PHP_ME(Module, C_FindObjects,             arginfo_C_FindObjects,             ZEND_ACC_PUBLIC)
    PHP_ME(Module, C_FindObjectsFinal,        arginfo_C_FindObjectsFinal,        ZEND_ACC_PUBLIC)
    PHP_ME(Module, C_GetAttributeValue,       arginfo_C_GetAttributeValue,       ZEND_ACC_PUBLIC)
    PHP_ME(Module, C_CopyObject,              arginfo_C_CopyObject,              ZEND_ACC_PUBLIC)
    PHP_ME(Module, C_DestroyObject,           arginfo_C_DestroyObject,           ZEND_ACC_PUBLIC)

    PHP_FE_END
};


DEFINE_MAGIC_FUNCS(pkcs11, module, Module)
