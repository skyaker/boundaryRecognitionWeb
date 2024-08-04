import streamlit as st
from io import BytesIO
import requests

st.title('Edge detector')

importedImage = st.file_uploader(label = 'Load image for processing', type = ['png', 'jpeg', 'jpg'])

if importedImage is not None :
  imageBytes = importedImage.getvalue()
  st.image(imageBytes, caption = 'Uploaded image', use_column_width = True)

  if st.button('Start processing') :
    try : 
      response = requests.post(
        'http://localhost:8080/process',
        files = {'file' : ('image.jpg', BytesIO(imageBytes), 'image.jpeg')}
      )
      if response.status_code == 200 :
        processed_image = BytesIO(response.content)
        st.image(processed_image, caption='Processed Image', use_column_width=True)
      else :
        st.error(f"Error: {response.status_code}")
    except requests.exceptions.RequestException as e :
      st.error(f"Error: {e}")
else:
  st.info('Please upload an image to process.')