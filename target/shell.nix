let
  pkgs = import <nixpkgs> {}; 
  rtttl = ps: ps.callPackage ../rtttl.nix {};
  python = pkgs.python39.withPackages(ps: with ps; [
    (rtttl ps)
  ]);
in
pkgs.pkgsCross.msp430.mkShell {
    nativeBuildInputs = [ python pkgs.mspdebug ];
    buildInputs = [ ];
}