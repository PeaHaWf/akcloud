import { Box, Typography } from '@mui/material'
import File from '../components/File'
const Files = () => {
  return (
    <Box sx={{ m: 2.5 }}>
      <Typography variant="h5" fontWeight="bold" sx={{ pb: 3 }}>
        文件
      </Typography>
      <File />
    </Box>
  )
}
export default Files
