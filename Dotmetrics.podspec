Pod::Spec.new do |s|  
    s.name              = 'Dotmetrics'
    s.version           = '1.7.5'
    s.summary           = 'SDK for collecting app usage data'
    s.homepage          = 'https://www.dotmetrics.net'

    s.author            = { "Dotmetrics" => 'info@dotmetrics.net' }
    s.license           = { :type => 'Custom', :file => 'LICENSE' }

    s.platform          = :ios
    s.source       = { :git => "https://github.com/DotMetrics/iOSDistribution.git", :tag => "#{s.version}" }
    s.platform = :ios
    s.ios.deployment_target = '7.0'
    s.ios.vendored_frameworks = 'Dotmetrics.xcframework'

end
