{
  description = "Internet sockets";

  outputs = { self, nixpkgs }:

    let
      system = "x86_64-linux";
      pkgs = nixpkgs.legacyPackages.${system};

    in
    {
      defaultPackage.${system} = pkgs.stdenv.mkDerivation {
        name = "bmstu-os-02";

        buildInputs = with pkgs; [
          gcc
          man-pages
        ];

        src = ./.;
      };

      formatter.${system} = pkgs.nixpkgs-fmt;
    };
}
