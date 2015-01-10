class Caregiver < ActiveRecord::Base
	belongs_to :patient

	def patient
		Patient.find(patient_id)
	end

	def set_patient(patient)
		patient_id = patient.id
	end

end
