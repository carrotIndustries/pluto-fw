{ lib, buildPythonPackage, fetchPypi, pytestCheckHook, fetchpatch }:

buildPythonPackage rec {
  pname = "rtttl";
  version = "0.2";

  src = fetchPypi {
    inherit pname version;
    sha256 = "02h23mp9mvd9d7hsvj280k3drms9b1dbla4kar6m2fjdnhd9i9jq";
  };

  meta = with lib; {
    description = "Nokia Ring Tone Text Transfer Language (RTTTL) parser for python";
    homepage = "https://github.com/asdwsda/rtttl";
    license = licenses.gpl3;
  };
}