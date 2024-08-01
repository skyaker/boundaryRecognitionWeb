DO $$
BEGIN
  IF NOT EXISTS (SELECT 1 FROM pg_database WHERE datname = 'imageData') THEN
    EXECUTE 'CREATE DATABASE imageData';
  END IF;
END
$$;

DO $$
BEGIN
  IF NOT EXISTS (SELECT 1 FROM information_schema.tables WHERE table_name = 'images') THEN
    CREATE TABLE images (
    id SERIAL PRIMARY KEY,
    filename TEXT NOT NULL,
    upload_date TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    path TEXT NOT NULL
    );
  END IF;
END
$$;

DO $$
BEGIN
  IF NOT EXISTS (SELECT 1 FROM information_schema.tables WHERE table_name = 'processing_history') THEN
    CREATE TABLE processing_history (
    id SERIAL PRIMARY KEY,
    image_id INTEGER REFERENCES images(id),
    processing_date TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    parameters TEXT,
    result_path TEXT
    );
  END IF;
END
$$;

