const TOKEN_KEY = "WEBSITE-TOKEN"

export const getToken = () => {
  return localStorage.getItem(TOKEN_KEY)
}

export const setToken = (token: string) => {
  localStorage.setItem(TOKEN_KEY, token)
}

export const removeToken = () => {
  localStorage.removeItem(TOKEN_KEY)
}

export const cutImgBase64Prefix = (imageBase64: string) => {
  return imageBase64.replace(/^data:.*?;base64,/, '');
}