class CreateLocations < ActiveRecord::Migration
  def change
    create_table :locations do |t|
      t.belongs_to :patient
      t.decimal :latitude
      t.decimal :longitude

      t.timestamps
    end
  end
end
