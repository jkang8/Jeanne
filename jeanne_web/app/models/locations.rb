class Locations < ActiveRecord::Base

	def geo
		Geokit::LatLng.new(latitude, longitude)
	end



end
