let
  # unstable = import (fetchTarball https://nixos.org/channels/nixos-unstable/nixexprs.tar.xz) {};
  pkgs = import <nixpkgs> {};

  crossPkgs = import <nixpkgs> {
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
    vscode = pkgs.vscode;
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
    ];
  };
in
pkgs.mkShell {
    LD_LIBRARY_PATH = "${pkgs.mspds}/lib";
    nativeBuildInputs = with pkgs; [ crossPkgs.buildPackages.gcc crossPkgs.buildPackages.gdb mspds mspdebug zeromq python gnumake mbedtls pulseaudio pkgconfig gobjectIntrospection gtk3 lsof gdb vscode];
}