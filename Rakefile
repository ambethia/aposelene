require 'chunky_png'
require 'fileutils'
require 'yaml'

task :resources do
  manifest = YAML.load_file("Assets/manifest.yml")
  manifest["textures"].each do |file_name|
    dump_texture_resource("Assets/Textures/#{file_name}")
  end
  manifest["shaders"].each do |file_name|
    dump_shader_resource("Assets/Shaders/#{file_name}")
  end
  manifest["fonts"].each do |file_name|
    dump_font_resource("Assets/Fonts/#{file_name}")
  end
end

def dump_font_resource(path_to_file)
  name =  "#{File.basename(path_to_file, ".fnt")}"
  resource = "Resources/#{name}_font.h"
  dump_texture_resource(path_to_file.gsub(/\.fnt$/, ".png"), "#{name}_font")
  print("Generating #{resource}...")

  bmfont = File.open(path_to_file).read
  characters = []
  bmfont.lines.each do |line|
    if line =~ /char id=([-0-9]+)\s+x=([-0-9]+)\s+y=([-0-9]+)\s+width=([-0-9]+)\s+height=([-0-9]+)\s+xoffset=([-0-9]+)\s+yoffset=([-0-9]+)\s+xadvance=([-0-9]+)/
      # $~[1..8].map {|i| "%3d" % i}.join(", ")
      characters[$1.to_i] = "  {#{$~[1]}, {{#{$~[2]}, #{$~[3]}}, {#{$~[4]}, #{$~[5]}}}, {#{$~[6]}, #{$~[7]}}, #{$~[8]}}, // \"#{$1.to_i.chr}\""
    end
  end
  characters.map! { |line| line ||= "  {0, {{0, 0}, {0, 0}}, {0, 0}, 0}," }
  source = <<-EOF
//
//  #{name}_font.h
//  Aposelene
//
//  Copyright #{Date.today.year} Jason L Perry. All rights reserved.
//

#ifndef _#{name}_font_h
#define _#{name}_font_h

#include "#{name}_font_texture.h"

ASFontGlyph #{name}_font_glyphs[] = {
#{characters.join("\n")}
};

#endif
  
  EOF
  File.open(resource, 'w') { |file| file.write(source) }
  print "OK\n"
end

def dump_texture_resource(path_to_file, name = nil)
  name ||=  "#{File.basename(path_to_file, ".png")}"
  resource = "Resources/#{name}_texture.h"
  print("Generating #{resource}...")

  png = ChunkyPNG::Image.from_file(path_to_file)
  height = png.height
  width = png.width
  pixel_data = "".tap do |data|
    png.pixels.each_slice(4) do |row|
      data << row.map { |x| "0x%02x" % x }.join(", ") + ",\n    "
    end
  end

  path_to_atlas = path_to_file.gsub(/\.png$/, ".txt")
  atlas_data = File.exist?(path_to_atlas) ? File.open(path_to_atlas).read : ""

  animations = ""
  path_to_yml = path_to_file.gsub(/\.png$/, ".yml")
  if File.exist?(path_to_yml)
    yml = YAML.load_file(path_to_yml)
    yml.each do |anim|
      src = <<-EOF

static const ASSpriteAnimation _#{name}_#{anim["name"]}_animation = {
  &#{name}_atlasData[0], #{anim["fps"]}, #{anim["frames"].size},
  (int[#{anim["frames"].size}]){ #{anim["frames"].join(", ")} }
};
ASSpriteAnimation *#{name}_#{anim["name"]}_animation = (ASSpriteAnimation *)&_#{name}_#{anim["name"]}_animation;

      EOF
      animations << src
    end
  end
  source = <<-EOF
//
//  #{name}_texture.h
//  Aposelene
//
//  Copyright #{Date.today.year} Jason L Perry. All rights reserved.
//

#ifndef _#{name}_texture_h
#define _#{name}_texture_h

#include "aposelene.h"
#{atlas_data}#{animations}
static const ASTextureResource _#{name}_texture = {
  #{width}, #{height},
  (unsigned int[#{height * width}]){
    #{pixel_data}}
};
ASTextureResource *#{name}_texture = (ASTextureResource *)&_#{name}_texture;
#endif

  EOF
  File.open(resource, 'w') { |file| file.write(source) }
  print "OK\n"
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
    <string>Resources/Icon.png</string>
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
