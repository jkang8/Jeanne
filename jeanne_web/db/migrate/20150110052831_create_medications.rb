class CreateMedications < ActiveRecord::Migration
  def change
    create_table :medications do |t|
	  t.string :name
	  t.string :description
	  t.integer :amount
	  t.belongs_to :patient
	  t.string :times
	  t.string :days

      t.timestamps
    end
  end
end
