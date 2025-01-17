--TEST--
Derive using ECDH P-384
--SKIPIF--
<?php

require_once 'require-userpin-login.skipif.inc';

if (!in_array(Pkcs11\CKM_ECDH1_DERIVE, $module->getMechanismList((int)getenv('PHP11_SLOT')))) {
	echo 'skip: CKM_ECDH1_DERIVE not supported ';
}

$info = $module->getInfo();
if (trim($info["manufacturerID"]) == 'SoftHSM'
 && $info["libraryVersion"]["major"] == 2
 && $info["libraryVersion"]["minor"] == 2
) {
	echo 'skip: Known bug in this version of SoftHSM';
}

?>
--FILE--
<?php

declare(strict_types=1);

$module = new Pkcs11\Module(getenv('PHP11_MODULE'));
$session = $module->openSession((int)getenv('PHP11_SLOT'), Pkcs11\CKF_RW_SESSION);
$session->login(Pkcs11\CKU_USER, getenv('PHP11_PIN'));

// P-384
$domainParameters = hex2bin('06052B81040022');
$rawPublickeyOther = hex2bin('049f0a09e8a6fc87f4804642c782b2cd3e566b3e62262090d94e12933a00916f559b62ea33197706a302f0722b781a9349ea8f0f2bcea854cdcf5d9ff0e0a19c3c35d63578292307d1d83031c0134700c2990ed5b38f6c92245103c2c1352132a3');

$keypair = $session->generateKeyPair(new Pkcs11\Mechanism(Pkcs11\CKM_EC_KEY_PAIR_GEN), [
	Pkcs11\CKA_LABEL => "Test ECDH Public",
	Pkcs11\CKA_EC_PARAMS => $domainParameters,
],[
	Pkcs11\CKA_TOKEN => false,
	Pkcs11\CKA_PRIVATE => true,
	Pkcs11\CKA_SENSITIVE => true,
	Pkcs11\CKA_DERIVE => true,
	Pkcs11\CKA_LABEL => "Test ECDH Private",
]);

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
string(64) "%x"
string(32) "%x"
