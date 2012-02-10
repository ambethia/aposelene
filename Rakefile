require 'yaml'
require 'fileutils'

require 'chunky_png'

task :resources do
  manifest = YAML.load_file("Assets/manifest.yml")
  manifest["textures"].each do |file_name|
    dump_texture_resource("Assets/Textures/#{file_name}")
  end
  manifest["shaders"].each do |file_name|
    dump_shader_resource("Assets/Shaders/#{file_name}")
  end
end

def dump_shader_resource(path_to_file)
  name =  "#{File.basename(path_to_file, ".glsl")}_shader"
  resource = "Resources/#{name}.h"
  print("Generating #{resource}...")

  glsl = File.open(path_to_file).read
  data = "".tap do |data|
    glsl.each_line do |line|
      unless line =~ /^\/\/|^\s*$/
        data << %[\n  "#{line.chomp}\\n"]
      end
    end
  end
  source = <<-EOF
//
//  #{name}.h
//  Aposelene
//
//  Generated #{Time.now}
//  Copyright #{Date.today.year} Jason L Perry. All rights reserved.
//

#ifndef _#{name}_h
#define _#{name}_h

static const char *#{name} =#{data};
#endif

  EOF
  File.open(resource, 'w') { |file| file.write(source) }
  print "OK\n"
end

def dump_texture_resource(path_to_file)
  name =  "#{File.basename(path_to_file, ".png")}_texture"
  resource = "Resources/#{name}.h"
  print("Generating #{resource}...")

  png = ChunkyPNG::Image.from_file(path_to_file)
  height = png.height
  width = png.width
  data = "".tap do |data|
    png.pixels.each_slice(4) do |row|
      data << row.map { |x| "0x%02x" % x }.join(", ") + ",\n  "
    end
  end
  source = <<-EOF
//
//  #{name}.h
//  Aposelene
//
//  Generated #{Time.now}
//  Copyright #{Date.today.year} Jason L Perry. All rights reserved.
//

#ifndef _#{name}_h
#define _#{name}_h

static const struct {
  int height;
  int width;
  unsigned int pixels[#{height * width}];
} #{name} = {
  #{height}, #{width}, {
  #{data}}
};

#endif

  EOF
  File.open(resource, 'w') { |file| file.write(source) }
  print "OK\n"
end

desc "Bundle a MacOS X App for the Demo"
task :bundle do
  require 'erb'
  
  bundle_contents = File.join("Build", "Demo.app", "Contents")
  FileUtils.rm_rf bundle_contents if File.exist? bundle_contents
  macos_dir = File.join(bundle_contents, "MacOS")
  resources_dir = File.join(bundle_contents, "Resources")
  FileUtils.mkdir_p macos_dir
  FileUtils.mkdir_p resources_dir
  
  plist = <<-PLIST
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1.0.dtd">
<plist version="1.0">
  <dict>
    <key>CFBundleExecutable</key>
    <string>Demo</string>
    <key>CFBundleIconFile</key>
    <string>Icon.png</string>
    <key>CFBundleIdentifier</key>
    <string>com.ambethia.aposelene.demo</string>
    <key>CFBundleInfoDictionaryVersion</key>
    <string>6.0</string>
    <key>CFBundlePackageType</key>
    <string>APPL</string>
    <key>CFBundleSignature</key>
    <string>????</string>
    <key>CFBundleVersion</key>
    <string>0.1</string>
    <key>LSMinimumSystemVersion</key>
    <string>10.5</string>
  </dict>
</plist>
  PLIST
  File.open(File.join(bundle_contents, "Info.plist"), "w") { |f| f.write(plist) }

  FileUtils.cp "demo", File.join(macos_dir, "Demo")
    
  FileUtils.cp File.join("Assets/Icon.png"), resources_dir  
end

task :default => :resources
