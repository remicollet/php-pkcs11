--TEST--
Derive using X25519
--SKIPIF--
<?php

require_once 'require-userpin-login.skipif.inc';

if (!in_array(Pkcs11\CKM_ECDH1_DERIVE, $module->getMechanismList((int)getenv('PHP11_SLOT')))) {
	echo 'skip: CKM_ECDH1_DERIVE not supported ';
}

if (!in_array(Pkcs11\CKM_EC_EDWARDS_KEY_PAIR_GEN, $module->getMechanismList((int)getenv('PHP11_SLOT')))) {
	echo 'skip: CKM_EC_EDWARDS_KEY_PAIR_GEN not supported ';
}

?>
--FILE--
<?php

declare(strict_types=1);

$module = new Pkcs11\Module(getenv('PHP11_MODULE'));
$session = $module->openSession((int)getenv('PHP11_SLOT'), Pkcs11\CKF_RW_SESSION);
$session->login(Pkcs11\CKU_USER, getenv('PHP11_PIN'));

$domainParameters = hex2bin('06032B656E'); // X25519
$rawPublickeyOther = hex2bin('0420715bbc7a82f99613f23580cdf87e0ff179524201fdad7d7d389529e6cb0ad25c');

// SoftHSMv2 uses CKM_EC_EDWARDS_KEY_PAIR_GEN instead of CKM_EC_MONTGOMERY_KEY_PAIR_GEN
$keypair = $session->generateKeyPair(new Pkcs11\Mechanism(Pkcs11\CKM_EC_EDWARDS_KEY_PAIR_GEN), [
	Pkcs11\CKA_LABEL => "Test X25519 Public",
	Pkcs11\CKA_EC_PARAMS => $domainParameters,
],[
	Pkcs11\CKA_TOKEN => false,
	Pkcs11\CKA_PRIVATE => true,
	Pkcs11\CKA_SENSITIVE => true,
	Pkcs11\CKA_DERIVE => true,
	Pkcs11\CKA_LABEL => "Test X25519 Private",
]);

var_dump(bin2hex($keypair->pkey->getAttributeValue([
	Pkcs11\CKA_EC_POINT,
])[Pkcs11\CKA_EC_POINT]));

$shared = '';

// SoftHSM2 only supports CKD_NULL
$params = new Pkcs11\Ecdh1DeriveParams(Pkcs11\CKD_NULL, $shared, $rawPublickeyOther);
$mechanism = new Pkcs11\Mechanism(Pkcs11\CKM_ECDH1_DERIVE, $params);
$secret = $keypair->skey->derive($mechanism, [
	Pkcs11\CKA_TOKEN => false,
	Pkcs11\CKA_CLASS => Pkcs11\CKO_SECRET_KEY,
	Pkcs11\CKA_KEY_TYPE => Pkcs11\CKK_AES,
	Pkcs11\CKA_SENSITIVE => false,
	Pkcs11\CKA_EXTRACTABLE => true,
	Pkcs11\CKA_ENCRYPT => true,
	Pkcs11\CKA_DECRYPT => true,
]);

var_dump(bin2hex($secret->getAttributeValue([
	Pkcs11\CKA_VALUE,
])[Pkcs11\CKA_VALUE]));

$iv = random_bytes(16);
$data = 'Hello World!';
$mechanism = new Pkcs11\Mechanism(Pkcs11\CKM_AES_CBC_PAD, $iv);
$ciphertext = $secret->encrypt($mechanism, $data);
var_dump(bin2hex($ciphertext));

$session->logout();

?>
--EXPECTF--
string(68) "%x"
string(64) "%x"
string(32) "%x"
