{
  description = "A Splatoon Save Editor";

  inputs.nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";

  outputs =
    { self, nixpkgs }:
    let
      forAllSystems = nixpkgs.lib.genAttrs [
        "x86_64-linux"
        "aarch64-linux"
      ];
    in
    {
      packages = forAllSystems (
        system:
        let
          pkgs = nixpkgs.legacyPackages.${system};
        in
        {
          default = pkgs.stdenv.mkDerivation {
            pname = "spoon-save-editor";
            version = "2.0.1";

            src = self;

            nativeBuildInputs = with pkgs; [
              cmake
              qt6.wrapQtAppsHook
              copyDesktopItems
            ];

            desktopItems = [
              (pkgs.makeDesktopItem {
                name = "spoon_save_editor";
                desktopName = "Spoon Save Editor";
                comment = "Save Editor for Splatoon";
                exec = "spoon_save_editor";
                icon = "spoon_save_editor";
                categories = [ "Game" "Utility" ];
                terminal = false;
              })
            ];

            postInstall = ''
              install -Dm644 ${self}/images/icon.png \
                $out/share/icons/hicolor/256x256/apps/spoon_save_editor.png
            '';

            buildInputs = with pkgs; [
              qt6.qtbase
            ];

            meta = with pkgs.lib; {
              description = "Save Editor for Splatoon";
              homepage = "https://github.com/tvyrval/Spoon-Save-Editor";
              license = licenses.gpl3Plus;
              mainProgram = "spoon_save_editor";
              platforms = platforms.linux;
            };
          };
        }
      );

      apps = forAllSystems (system: {
        default = {
          type = "app";
          program = "${self.packages.${system}.default}/bin/spoon_save_editor";
        };
      });

      devShells = forAllSystems (
        system:
        let
          pkgs = nixpkgs.legacyPackages.${system};
        in
        {
          default = pkgs.mkShell {
            packages = with pkgs; [
              cmake
              ninja
              qt6.qtbase
            ];
          };
        }
      );
    };
}
