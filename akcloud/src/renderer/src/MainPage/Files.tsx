import { Box, Grid, Typography } from '@mui/material'
import File from '../components/File'
import AddFile from '@renderer/components/AddFile'

const Files = () => {
  return (
    <Box sx={{ m: 2.5, width: '100%' }}>
      <Box sx={{ display: 'flex' }}>
        <Typography variant="h5" fontWeight="bold" sx={{ pb: 3 }}>
          文件
        </Typography>
        <Box sx={{ flexGrow: 1 }} />
        <AddFile />
      </Box>
      <Box sx={{ display: 'flex', flexWrap: 'wrap', gap: 2 }}>
        <File />
        <File />
        <File />
      </Box>
    </Box>
  )
}
export default Files
