with import <nixpkgs>{};

pkgsCross.msp430.mkShell {
    nativeBuildInputs = [ meson mspdebug  zeromq python37Full python37Packages.pygobject3 msp430GccSupport   mspdebug gnumake mbedtls pulseaudio python37Packages.pip python37Packages.setuptools python37Packages.pyzmq alsaLib pkgconfig gobject-introspection gobjectIntrospection gtk3 pango cairo];
    buildInputs = [ msp430Newlib ];
}