// swift-tools-version:5.3
import PackageDescription

let package = Package(
    name: "Dotmetrics",
    platforms: [
        .iOS(.v8)
    ],
    products: [
        // Products define the executables and libraries a package produces, and make them visible to other packages.
        .library(
            name: "Dotmetrics",
            targets: ["Dotmetrics"]),
    ],
    dependencies: [],
    targets: [
        .binaryTarget(
                    name: "Dotmetrics",
                    url: "https://github.com/DotMetrics/iOSDistribution/releases/download/1.7.5/Dotmetrics.xcframework.zip",
                    checksum: "e1b150b1d29e85976d55f1238cee258dc2b5909b8a95cd8204273b739a2f4f1e"
                )
    ]
)
