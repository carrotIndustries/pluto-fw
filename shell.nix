with import <nixpkgs>{};
with pkgs;
let
  rtttl = ps: ps.callPackage ./rtttl.nix {};
  python = python39.withPackages(ps: with ps; [
    (rtttl ps)
    pygobject3
    pyzmq
  ]);
  vscode = vscode-with-extensions.override {
    vscodeExtensions = with pkgs.vscode-extensions; [
      bbenoist.Nix
      ms-vscode.cpptools
      ms-python.python
    ] ++ pkgs.vscode-utils.extensionsFromVscodeMarketplace [{
      name = "beyond-debug";
      publisher = "coolchyni";
      version = "0.9.6";
      sha256 = "1g5qcblnz67786aaqzszrxx6qc80bylyb87aqlgk6423zv4aksq4";
    }];
  };
in
mkShell {
    nativeBuildInputs = [ zeromq python gnumake mbedtls pulseaudio pkgconfig gobjectIntrospection gtk3 gdb vscode ];
}
