# Interceptor-Gamepad-Controller — SDK et installation

Projet : [Interceptor-Gamepad-Controller](https://github.com/TitanCSA/Interceptor-Gamepad-Controller). [Sources et documentation des SDK](https://github.com/TitanCSA/sdk-packages/tree/main/projects/Interceptor-Gamepad-Controller).

Choisis le paquet a installer. Installe son outil habituel (Node.js/npm, Python/pip, .NET, etc.), puis execute la commande dans le dossier de ton application. Les pilotes et applications requis restent a installer separement selon le README du projet.

Les SDK natifs sont destines a Windows x64. Java utilise Java22+ (Kotlin passe par le SDK Java). Flutter utilise le SDK Dart ; le chargement de la DLL doit etre configure dans l application. AutoHotInterceptor .NET utilise .NET Framework4.8.

| Paquet | Registre | Disponibilite | Installation / utilisation |
| --- | --- | --- | --- |
| `interceptor-gamepad-controller` | vcpkg | Registre TitanCSA disponible (configuration ci-dessous) | `vcpkg install interceptor-gamepad-controller:x64-windows` |
| `interceptor-gamepad-controller` | Conan | Registre TitanCSA disponible (configuration ci-dessous) | `conan install --requires=interceptor-gamepad-controller/1.0.13 -r titancsa --build=missing -s compiler.cppstd=17` |
| `Interceptor-Gamepad-Controller.Native` | NuGet | Publie et verifie | `Install-Package Interceptor-Gamepad-Controller.Native -Version 1.0.13` |
| `InterceptorGamepadController.SDK` | NuGet | Publie et verifie | `dotnet add package InterceptorGamepadController.SDK --version 1.0.0` |
| `interceptor_gamepad_controller` | PyPI | Pas encore disponible / validation en cours | Commande prevue apres publication : `python -m pip install interceptor-gamepad-controller==1.0.0` |
| `interceptor-gamepad-controller` | Conda | Canal TitanCSA disponible ; acceptation conda-forge encore en attente | `conda install --override-channels -c https://raw.githubusercontent.com/TitanCSA/sdk-packages/089bc73095aa8eda89bacbe10d0b9e58394b0207/conda-channel -c conda-forge interceptor-gamepad-controller` |
| `interceptor_gamepad_controller` | crates.io | Publie et verifie | `cargo add interceptor-gamepad-controller@1.0.0` |
| `interceptor_gamepad_controller` | npm | Publie et verifie | `npm install interceptor-gamepad-controller@1.0.0` |
| `io.github.titancsa:interceptor-gamepad-controller` | Maven Central | Publie et verifie | `io.github.titancsa:interceptor-gamepad-controller:1.0.0 (pom.xml / Gradle)` |
| `com.github.TitanCSA:Interceptor-Gamepad-Controller` | JitPack | Publie et verifie | `com.github.TitanCSA:Interceptor-Gamepad-Controller:java-sdk-v1.0.0 (avec le depot JitPack)` |
| `github.com/TitanCSA/Interceptor-Gamepad-Controller/Interceptor-Gamepad-Controller-Go-modules` | Go Modules | Publie et verifie | `go get github.com/TitanCSA/Interceptor-Gamepad-Controller/Interceptor-Gamepad-Controller-Go-modules@v1.0.0` |
| `github.com/TitanCSA/Interceptor-Gamepad-Controller/Interceptor-Gamepad-Controller-Go-modules` | pkg.go.dev | Publie et verifie | `Documentation : https://pkg.go.dev/github.com/TitanCSA/Interceptor-Gamepad-Controller/Interceptor-Gamepad-Controller-Go-modules@v1.0.0` |
| `interceptor_gamepad_controller` | PowerShell Gallery | Publie et verifie | `Install-Module InterceptorGamepadController -RequiredVersion 1.0.0 -Scope CurrentUser` |
| `interceptor_gamepad_controller` | pub.dev | Publie et verifie | `dart pub add interceptor_gamepad_controller:1.0.0` |
| `interceptor_gamepad_controller` | RubyGems | Publie et verifie | `gem install interceptor-gamepad-controller -v 1.0.0` |
| `interceptor_gamepad_controller` | Packagist | Pas encore disponible / validation en cours | Commande prevue apres publication : `composer require titancsa/interceptor-gamepad-controller:1.0.0` |

## Configuration vcpkg et Conan

Les sources et headers C / C++ sont dans le dossier central de ce projet. Les headers C se trouvent dans les dossiers `*-C-ffi` ou `*-C-json` (Interceptor-MultiLang : `native/`). Les paquets natifs C++ et la disponibilite du bridge C sont distincts ; consulte leur README avant de lier une application C.

Pour vcpkg, ajoute un fichier `vcpkg-configuration.json` a cote de ton manifeste `vcpkg.json` :

```json
{
  "registries": [
    {
      "kind": "git",
      "repository": "https://github.com/TitanCSA/vcpkg-registry",
      "baseline": "9a4b8cb920d4cfc97ce6d7af461bcc7508392a68",
      "packages": [
        "interceptor-gamepad-controller",
        "interceptor-gamepad-controller-sdk"
      ]
    }
  ]
}
```

Pour Conan2, configure une seule fois l index source :

```powershell
git clone https://github.com/TitanCSA/sdk-packages.git
conan remote add titancsa ./sdk-packages/conan-index -t local-recipes-index
```

Execute ensuite la commande Conan du tableau. Ce registre construit les bibliotheques depuis leurs sources ; il est distinct de ConanCenter. La commande NuGet native s execute dans la console du gestionnaire de packages de Visual Studio.

## Configuration Maven et JitPack

Dans le bloc `<dependencies>` de ton `pom.xml` :

```xml
<dependency>
  <groupId>io.github.titancsa</groupId>
  <artifactId>interceptor-gamepad-controller</artifactId>
  <version>1.0.0</version>
</dependency>
```

Avec Gradle Kotlin :

```kotlin
implementation("io.github.titancsa:interceptor-gamepad-controller:1.0.0")
```

Pour JitPack, ajoute aussi le depot `https://jitpack.io`, puis utilise les coordonnees du tableau. Les versions Go et JitPack deja publiees continuent de pointer vers leurs anciens tags immuables dans le depot du projet : ces tags restent disponibles. Leurs sources de travail sont maintenant centralisees ici ; aucun ancien import Go n est renomme.

## Etat et documentation

Les commandes marquees « prevues » ne sont pas encore des installations disponibles. Le canal Conda TitanCSA n est pas le catalogue officiel conda-forge. Les versions et licences existantes ne sont pas remplacees par cette reorganisation.

Les guides API et exemples de chaque langage se trouvent dans leurs dossiers SDK, a cote des sources. Les bindings peuvent exposer une partie de l API C++ ; les limites restent documentees dans chaque SDK.

[Retour au catalogue central](../../README.md)
