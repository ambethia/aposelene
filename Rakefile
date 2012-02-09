require 'yaml'
require 'chunky_png'

task :resources do
  manifest = YAML.load_file("Assets/manifest.yml")
  manifest["textures"].each do |file_name|
    dump_texture_resource("Assets/Textures/#{file_name}")
  end
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

task :default => :resources
