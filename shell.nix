let
  unstable = import (fetchTarball https://nixos.org/channels/nixos-unstable/nixexprs.tar.xz) {};
  pkgs = import <nixpkgs> {};
  crossPkgs = import <nixpkgs> {
    # crossSystem = (import <nixpkgs/lib>).systems.examples.msp430;
  crossSystem = {
    config = "msp430-elf";
    libc = "newlib";
    gcc.float = " --enable-multilib --disable-libquadmath ";
  };
  };
  rtttl = ps: ps.callPackage ./rtttl.nix {};
  python = pkgs.python39.withPackages(ps: with ps; [
    (rtttl ps)
    pygobject3
    pyzmq
  ]);
  vscode = pkgs.vscode-with-extensions.override {
    vscode = unstable.vscode;
    vscodeExtensions = [
      pkgs.vscode-extensions.bbenoist.Nix
      pkgs.vscode-extensions.ms-vscode.cpptools
    ] ++ pkgs.vscode-utils.extensionsFromVscodeMarketplace [{
      name = "msp430-assembly";
      publisher = "zacharypenn";
      version = "1.0.0";
      sha256 = "1pjx57bsjkkly0110drhgxycx17m2hvgn5ppcawwsg0gg86k9v2a";
    }
    {
      name = "linkerscript";
      publisher = "zixuanwang";
      version = "1.0.2";
      sha256 = "0rr5mz8g8myskgixiw76rwda8g955a1al8kk4s30b0byfaszia17";
    }
    {
      name = "gnu-mapfiles";
      publisher = "trond-snekvik";
      version = "1.1.0";
      sha256 = "0416qprq9rnzl9na3fxhs9wnmws9zfm473qxnvji2vy752l4wxr4";
    }
    # {
    #   name = "debug";
    #   publisher = "webfreak";
    #   version = "0.25.1";
    #   sha256 = "1l01sv6kwh8dlv3kygkkd0z9m37hahflzd5bx1wwij5p61jg7np9";
    # }
    # {
    #   name = "vgdb";
    #   publisher = "penagos";
    #   version = "1.0.0";
    #   sha256 = "0hbl21p0clcwfb5jrrdijsw6z58sddvsy6hyl8p39sq76mga4kj1";
    # }
    {
      name = "beyond-debug";
      publisher = "coolchyni";
      version = "0.9.6";
      sha256 = "1g5qcblnz67786aaqzszrxx6qc80bylyb87aqlgk6423zv4aksq4";
      sha256 = "0qjllvg2wwy3w8gg7q9dkbs0mccxxdq8cg59pbvw0vd8rxn71jpw";
    }
    ];
  };
in
pkgs.mkShell {
    LD_LIBRARY_PATH = "${pkgs.mspds}/lib";
    nativeBuildInputs = with pkgs; [ crossPkgs.buildPackages.gcc crossPkgs.buildPackages.gdb mspds mspdebug zeromq python gnumake mbedtls pulseaudio pkgconfig gobjectIntrospection gtk3 gdb vscodium clang ];
    nativeBuildInputs = with pkgs; [ crossPkgs.newlib crossPkgs.buildPackages.gcc crossPkgs.buildPackages.gdb mspds mspdebug zeromq python gnumake mbedtls pulseaudio pkgconfig gobjectIntrospection gtk3 gdb vscode];
}