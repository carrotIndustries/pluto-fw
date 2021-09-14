with import <nixpkgs>{};
with pkgs;
let
  rtttl = ps: ps.callPackage ./rtttl.nix {};
  python = python39.withPackages(ps: with ps; [
    (rtttl ps)
    pygobject3
    pyzmq
  ]);
in
mkShell {
    nativeBuildInputs = [ zeromq python gnumake mbedtls pulseaudio pkgconfig gobjectIntrospection gtk3];
}