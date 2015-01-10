class CreateCaregiversPatients < ActiveRecord::Migration
  def change
    create_table :caregivers_patients do |t|
    	t.integer :caregiver_id
    	t.integer :patient_id

      t.timestamps
    end
  end
end
