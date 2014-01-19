require 'date'
require 'erb'
require 'ostruct'

class String
	def to_deg
		lat_re = /(\d\d)(\d\d\.\d+)/
		lng_re = /(\d\d\d)(\d\d\.\d+)/
		
		if self =~ lat_re
			captures = lat_re.match(self)
			degrees = captures[1].to_f
			minutes_part = captures[2].to_f
			
			degrees + (minutes_part / 60.0)
		elsif self =~ lng_re
			captures = lng_re.match(self)
			degrees = captures[1].to_f
			minutes_part = captures[2].to_f
			
			degrees + (minutes_part / 60.0)
		else
			nil
		end
	end
end

# ---------------------------------------------------

def parse_nmea_0183_line(line)
	# format1 = '$GPRMC,hhmmss.ss,A,GGMM.M***,P,ggmm.m***,J,v.v*,b**.b*,ddmmyy,x.x,n,m*hh<CR><LF>'
	# format2 = '$GPRMC,hhmmss.ss,A,GGMM.M***,P,ggmm.m***,J,v.v*,b**.b*,ddmmyy'
	# example:   $GPRMC,164009.00,A,5030.3623,N,3027.2158,E,0.00,195.64,191213
	
	# (1) «GP» — идентификатор источника; в приведенном примере это GPS, «GL» - ГЛОНАСС, «GA» - Галилео, «GN» - ГНСС и т.п.
	# «RMC» — «Recommended Minimum sentence C»
	# (2,3,4) «hhmmss.ss» — время фиксации местоположения по Всемирному координированному времени UTC: «hh» — часы, «mm» — минуты, «ss.ss» — секунды. Длина дробной части секунд варьируется. Лидирующие нули не опускаются.
	# (5) «A» — статус: «A» — данные достоверны, «V» — недостоверны.
	# (6) «GGMM.MM» — широта. 2 цифры градусов(«GG»), 2 цифры целых минут, точка и дробная часть минут переменной длины. Лидирующие нули не опускаются.
	# (7) «P» — «N» для северной или «S» для южной широты.
	# (8) «gggmm.mm» — долгота. 3 цифры градусов(«ggg»), 2 цифры целых минут, точка и дробная часть минут переменной длины. Лидирующие нули не опускаются.
	# (9) «J» — «E» для восточной или «W» для западной долготы.
	# (10) «v.v» — горизонтальная составляющая скорости относительно земли в узлах. Число с плавающей точкой. Целая и дробная части переменной длины.
	# (11) «b.b» — путевой угол (направление скорости) в градусах. Число с плавающей точкой. Целая и дробная части переменной длины. Значение равное 0 соответствует движению на север, 90 — восток, 180 — юг, 270 — запад.
	# (12,13,14) «ddmmyy» — дата: день месяца, месяц, последние 2 цифры года (ведущие нули обязательны).
	
	#       (1----------)    (2---)(3---)(4-------) (5----) (6-----------) (7----) (8-----------) (9----) (10------) (11------) (12--)(13--)(14--)
	re = /\$(GP|GL|GA|GN)RMC,(\d\d)(\d\d)(\d+\.\d+),([A-Z]),(\d+\d\d\.\d+),([A-Z]),(\d+\d\d\.\d+),([A-Z]),(\d+\.\d+),(\d+\.\d+),(\d\d)(\d\d)(\d\d)/
	
	matches = re.match line
	{
		:source => matches[1],
		:pos_fixed_at => DateTime.strptime("#{ matches[2] }.#{ matches[3] }.#{ matches[4].to_i }", '%H.%M.%S'),
		:valid? => (matches[5].upcase == 'A') ? true : false,
		:lat => matches[6].to_deg,
		:lat_direction => matches[7],
		:lng => matches[8].to_deg,
		:lng_direction => matches[9],
		:velocity => matches[10].to_f,
		:velocity_angle => matches[11].to_f,
		:date => DateTime.strptime("#{ matches[12] }.#{ matches[13] }.#{ matches[14] }", '%d.%m.%y')
	}
end

# ---------------------------------------------------

lines = File.read('gprmc.txt')

waypoints = lines.split(/\n/).map do |line|
	parse_nmea_0183_line(line)
end

def render_erb(template, locals)
  ERB.new(template).result(OpenStruct.new(locals).instance_eval { binding })
end

erb = <<-ERB
	window.waypoints = [
		<% waypoints.each do |p| %>
			[ <%= p[:lat] %>, <%= p[:lng] %> ],
		<% end %>
	];
ERB

File.open('waypoints_converted.js', 'w') do |f|
	f.write render_erb(erb, { :waypoints => waypoints })
end